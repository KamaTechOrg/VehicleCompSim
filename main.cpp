#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world454d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world454.lib")
#endif

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <numeric>  // For std::accumulate

using namespace cv;
using namespace std;


//original values:
// vector<Point> points = { Point(200, height), Point(550, 250), Point(1100, height) };
// HoughLinesP(cropped_canny, lines, 2, CV_PI / 180, 100, 40, 5);
//
//Elimelech's' values:
//std::vector<cv::Point> points = { cv::Point(200, height - 30), cv::Point(610, 530), cv::Point(1200, height - 30) };
//HoughLinesP(cropped_canny, lines, 2, CV_PI / 180, 100, 2, 50);


const int LANE_CENTER_SMOOTHING_FRAMES = 1;  // Number of frames to smooth lane center calculation
vector<int> lane_center_history;  // History of lane centers for smoothing

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

// Function to display lines on the image
Mat display_lines(Mat img, const vector<vector<int>>& lines) {
    Mat line_image = Mat::zeros(img.size(), img.type());
    for (const auto& line_points : lines) {
        cv::line(line_image, Point(line_points[0], line_points[1]), Point(line_points[2], line_points[3]), Scalar(255, 0, 0), 10);
    }
    return line_image;
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

// Function to calculate a smoothed lane center using moving average
int get_smoothed_lane_center(int current_lane_center) {
    lane_center_history.push_back(current_lane_center);
    if (lane_center_history.size() > LANE_CENTER_SMOOTHING_FRAMES) {
        lane_center_history.erase(lane_center_history.begin());
    }
    int smoothed_center = accumulate(lane_center_history.begin(), lane_center_history.end(), 0) / lane_center_history.size();
    return smoothed_center;
}

// Function to check for lane departure
void check_lane_departure(const vector<vector<int>>& averaged_lines, int frame_width) {
    if (averaged_lines.size() != 2) {
        // cout << "Warning: Unable to detect both lane lines!" << endl;
        return;
    } // cout << "Both lines detected" << endl;

    int left_x2 = averaged_lines[0][2];  // x-coordinate of the right end of the left lane line
    int right_x2 = averaged_lines[1][2]; // x-coordinate of the left end of the right lane line
    int lane_center = (left_x2 + right_x2) / 2;  // Center of the detected lane
    int frame_center = frame_width / 2;  // Center of the frame

    // Calculate smoothed lane center
    int smoothed_lane_center = get_smoothed_lane_center(lane_center);

    int deviation = smoothed_lane_center - frame_center; // Deviation from the center

    if (abs(deviation) > 50) {  // Increased threshold for deviation
        cout << "Warning: Lane Departure Detected!" << endl;
    }
}

// Main function
int main() {
    VideoCapture cap("lane_vid2.mp4");
    if (!cap.isOpened()) {
        cerr << "Error opening video file" << endl;
        return -1;
    }

    while (cap.isOpened()) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cout << "Cannot read the frame from the video; the file may be corrupted or the video has ended." << endl;
            break;
        }

        Mat canny_image = canny(frame);
        Mat cropped_canny = region_of_interest(canny_image);
        vector<Vec4i> lines;
        HoughLinesP(cropped_canny, lines, 2, CV_PI / 180, 100, 2, 50);
        vector<vector<int>> averaged_lines = average_slope_intercept(frame, lines);
        Mat line_image = display_lines(frame, averaged_lines);
        Mat combo_image;
        addWeighted(frame, 0.8, line_image, 1, 1, combo_image);

        // Check for lane departure
        check_lane_departure(averaged_lines, frame.cols);

        imshow("result", combo_image);
        if (waitKey(1) == 'q') break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}