#include "laneDetction.h"
#include <numeric>  // For std::accumulate

// Constants
// The height of every frame is 720, so 690 is 30 px above the bottom
const cv::Point POINT1(200, 690);  
const cv::Point POINT2(610, 530);
const cv::Point POINT3(1200, 690);


// Function to perform polynomial fit (linear regression)
std::pair<float, float> polyfit(const std::vector<float>& x, const std::vector<float>& y) {
	int n = x.size();
	float x_sum = accumulate(x.begin(), x.end(), 0.0f);
	float y_sum = accumulate(y.begin(), y.end(), 0.0f);
	float xy_sum = 0.0f;
	float x2_sum = 0.0f;

	for (int i = 0; i < n; ++i) {
		xy_sum += x[i] * y[i];
		x2_sum += x[i] * x[i];
	}

	// Slope (m) and Intercept (c)
	float slope = (n * xy_sum - x_sum * y_sum) / (n * x2_sum - x_sum * x_sum);
	float intercept = (y_sum - slope * x_sum) / n;

	return std::make_pair(slope, intercept);
}

// Function to create points for the line
std::vector<int> make_points(const cv::Mat& image, const std::pair<float, float>& line) {
	float slope = line.first, intercept = line.second;
	int y1 = image.rows;  // Bottom of the image
	int y2 = static_cast<int>(y1 * 3 / 5);  // Slightly lower than the middle
	int x1 = static_cast<int>((y1 - intercept) / slope);
	int x2 = static_cast<int>((y2 - intercept) / slope);

	// Removed clamping to allow for natural line calculation
	return { x1, y1, x2, y2 };
}

// Function to compute the average slope and intercept for lines
std::vector<std::vector<int>> average_slope_intercept(const cv::Mat& image, const std::vector<cv::Vec4i>& lines) {
	std::vector<std::pair<float, float>> left_fit, right_fit;
	if (lines.empty()) {
		return {};
	}

	for (const auto& line : lines) {
		float x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
		auto fit = polyfit(std::vector<float>{x1, x2}, std::vector<float>{y1, y2});
		float slope = fit.first;
		float intercept = fit.second;

		// Remove strict slope constraints for more natural line detection
		if (slope < 0) {
			left_fit.push_back(fit);
		}
		else {
			right_fit.push_back(fit);
		}
	}

	std::pair<float, float> left_fit_average = { 0, 0 }, right_fit_average = { 0, 0 };

	if (!left_fit.empty()) {
		left_fit_average = accumulate(left_fit.begin(), left_fit.end(), std::make_pair(0.0f, 0.0f), [](const auto& acc, const auto& p) {
			return std::make_pair(acc.first + p.first, acc.second + p.second);
			});
		left_fit_average.first /= left_fit.size();
		left_fit_average.second /= left_fit.size();
	}

	if (!right_fit.empty()) {
		right_fit_average = accumulate(right_fit.begin(), right_fit.end(), std::make_pair(0.0f, 0.0f), [](const auto& acc, const auto& p) {
			return std::make_pair(acc.first + p.first, acc.second + p.second);
			});
		right_fit_average.first /= right_fit.size();
		right_fit_average.second /= right_fit.size();
	}

	std::vector<std::vector<int>> averaged_lines;
	if (!left_fit.empty()) {
		averaged_lines.push_back(make_points(image, left_fit_average));
	}
	if (!right_fit.empty()) {
		averaged_lines.push_back(make_points(image, right_fit_average));
	}

	return averaged_lines;
}

// Function to detect edges using Canny
cv::Mat canny(const cv::Mat& img) {
	cv::Mat gray, blur, edge;
	cvtColor(img, gray, cv::COLOR_RGB2GRAY);
	GaussianBlur(gray, blur, cv::Size(5, 5), 0);
	Canny(blur, edge, 50, 150);
	return edge;
}

// Function to display lines on the origen image
cv::Mat display_lines(cv::Mat& img, const std::vector<std::vector<int>>& lines) {
	cv::Mat line_image = cv::Mat::zeros(img.size(), img.type());
	for (const auto& line_points : lines) {
		cv::line(line_image, cv::Point(line_points[0], line_points[1]), cv::Point(line_points[2], line_points[3]), cv::Scalar(255, 0, 0), 10);
	}
	addWeighted(img, 0.8, line_image, 1, 1, img);
	return img;
}

// Function to define the region of interest
cv::Mat region_of_interest(const cv::Mat& img) {
	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());
	int height = img.rows;
	std::vector<cv::Point> points = { POINT1, POINT2, POINT3 };
	fillPoly(mask, std::vector<std::vector<cv::Point>>{points}, cv::Scalar(255, 255, 255));
	cv::Mat masked_image;
	bitwise_and(img, mask, masked_image);
	return masked_image;
}

std::vector<std::vector<int>> detect_lanes(cv::Mat& image) {
	cv::Mat canny_image = canny(image);  // Use the canny function to detect edges
	cv::Mat cropped_canny = region_of_interest(canny_image);  // Apply region of interest
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(cropped_canny, lines, 2, CV_PI / 180, 40, 20, 50);  // Detect lines

	std::vector<std::vector<int>> averaged_lines = average_slope_intercept(image, lines);  // Get averaged lines
	display_lines(image, averaged_lines);

	return averaged_lines;
}