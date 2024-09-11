#include "laneDetction.h"
#include <numeric>  // For std::accumulate

using namespace cv;
using namespace std;


// Function to perform polynomial fit (linear regression)
pair<float, float> polyfit(const vector<float>& x, const vector<float>& y) {
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

	return make_pair(slope, intercept);
}

// Function to create points for the line
vector<int> make_points(const Mat& image, const pair<float, float>& line) {
	float slope = line.first, intercept = line.second;
	int y1 = image.rows;  // Bottom of the image
	int y2 = static_cast<int>(y1 * 3 / 5);  // Slightly lower than the middle
	int x1 = static_cast<int>((y1 - intercept) / slope);
	int x2 = static_cast<int>((y2 - intercept) / slope);

	// Removed clamping to allow for natural line calculation
	return { x1, y1, x2, y2 };
}

// Function to compute the average slope and intercept for lines
vector<vector<int>> average_slope_intercept(const Mat& image, const vector<Vec4i>& lines) {
	vector<pair<float, float>> left_fit, right_fit;
	if (lines.empty()) {
		return {};
	}

	for (const auto& line : lines) {
		float x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
		auto fit = polyfit(vector<float>{x1, x2}, vector<float>{y1, y2});
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

	pair<float, float> left_fit_average = { 0, 0 }, right_fit_average = { 0, 0 };

	if (!left_fit.empty()) {
		left_fit_average = accumulate(left_fit.begin(), left_fit.end(), make_pair(0.0f, 0.0f), [](const auto& acc, const auto& p) {
			return make_pair(acc.first + p.first, acc.second + p.second);
			});
		left_fit_average.first /= left_fit.size();
		left_fit_average.second /= left_fit.size();
	}

	if (!right_fit.empty()) {
		right_fit_average = accumulate(right_fit.begin(), right_fit.end(), make_pair(0.0f, 0.0f), [](const auto& acc, const auto& p) {
			return make_pair(acc.first + p.first, acc.second + p.second);
			});
		right_fit_average.first /= right_fit.size();
		right_fit_average.second /= right_fit.size();
	}

	vector<vector<int>> averaged_lines;
	if (!left_fit.empty()) {
		averaged_lines.push_back(make_points(image, left_fit_average));
	}
	if (!right_fit.empty()) {
		averaged_lines.push_back(make_points(image, right_fit_average));
	}

	return averaged_lines;
}

// Function to detect edges using Canny
Mat canny(const Mat& img) {
	Mat gray, blur, edge;
	cvtColor(img, gray, COLOR_RGB2GRAY);
	GaussianBlur(gray, blur, Size(5, 5), 0);
	Canny(blur, edge, 50, 150);
	return edge;
}

// Function to display lines on the origen image
Mat display_lines(Mat& img, const vector<vector<int>>& lines) {
	Mat line_image = Mat::zeros(img.size(), img.type());
	for (const auto& line_points : lines) {
		cv::line(line_image, Point(line_points[0], line_points[1]), Point(line_points[2], line_points[3]), Scalar(255, 0, 0), 10);
	}
	addWeighted(img, 0.8, line_image, 1, 1, img);
	return img;
}

// Function to define the region of interest
Mat region_of_interest(const Mat& img) {
	Mat mask = Mat::zeros(img.size(), img.type());
	int height = img.rows;
	std::vector<cv::Point> points = { cv::Point(200, height - 30), cv::Point(610, 530), cv::Point(1200, height - 30) };
	fillPoly(mask, vector<vector<Point>>{points}, Scalar(255, 255, 255));
	Mat masked_image;
	bitwise_and(img, mask, masked_image);
	return masked_image;
}

vector<vector<int>> detect_lanes(Mat& image) {
	Mat canny_image = canny(image);  // Use the canny function to detect edges
	Mat cropped_canny = region_of_interest(canny_image);  // Apply region of interest
	vector<Vec4i> lines;
	HoughLinesP(cropped_canny, lines, 2, CV_PI / 180, 40, 20, 50);  // Detect lines

	vector<vector<int>> averaged_lines = average_slope_intercept(image, lines);  // Get averaged lines
	display_lines(image, averaged_lines);

	return averaged_lines;
}