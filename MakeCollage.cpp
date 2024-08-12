#include "MakeCollage.h"
#include <algorithm>
#include <iostream>
#include <numeric>
//=============================================================================
bool isOverlapSignificant(const cv::Rect& rect1, const cv::Rect& rect2, double threshold) {
    cv::Rect intersection = rect1 & rect2;

    if (intersection.area() == 0) {
        return false;
    }

    double area1 = rect1.area();
    double area2 = rect2.area();
    double intersectionArea = intersection.area();

    double overlap1 = intersectionArea / area1;
    double overlap2 = intersectionArea / area2;

    return (overlap1 >= threshold || overlap2 >= threshold);
}
//=============================================================================
cv::Mat optimizeCollageGreedy(cv::Mat& originalMat, std::vector<cv::Rect>& diff_boxes)
{

    std::sort(diff_boxes.begin(), diff_boxes.end(), [](const cv::Rect& a, const cv::Rect& b) {
        return (a.width * a.height) > (b.width * b.height);
        });

    std::deque<std::shared_ptr<Box>> boxes;
    for (auto& rect : diff_boxes)
        boxes.push_back(std::make_shared<Box>(rect));

    int limit = 416;
    for (auto& box : boxes) {
        if (box->rect.width > limit || box->rect.height > limit) {
            int max_leng = std::max(box->rect.width, box->rect.height);
            double percent = (double)limit / (double)max_leng;
            box->rect.width = box->rect.width * percent;
            box->rect.height = box->rect.height * percent;
            box->resized = true;
        }
    }
    int start = 0;
    int end = 0;
    while (end < boxes.size()) {
        start = end;

        std::vector<std::shared_ptr<Box>> results = arrangeRectanglesInSquare_FFT(boxes, limit, limit);
        cv::Mat collage = createCollage(originalMat, results, limit, limit);

        //cv::imshow("collage", collage);
        //cv::waitKey(0);
        std::vector<cv::Rect> detectedBoxes;

        for (auto& box : results) {
            detectedBoxes.push_back(box->rect);
        }

        reverseCollage(originalMat, collage, results, detectedBoxes);
    }
    return originalMat;
}
//=============================================================================
std::vector<std::shared_ptr<Box>> arrangeRectanglesInSquare_FFT(std::deque<std::shared_ptr<Box>>& boxes, int width, int height)
{
    // Prepare a vector to store the heights of each row
    std::vector<std::shared_ptr<Box>> results;
    std::vector<int> rowHeights;

    // Create a white square image
    cv::Mat squareMat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    // Initialize offsets
    int xOffset = 0; // X offset for the current row
    int yOffset = 0; // Y offset for the current row
    int currentRowHeight = 0;

    // Place rectangles into the square
    for (auto& box : boxes) {
        cv::Rect& rect = box->rect;

        // Check if the rectangle fits in the current row
        if (xOffset + rect.width > width) {
            // Move to the next row
            yOffset += currentRowHeight;
            xOffset = 0;
            currentRowHeight = 0;
        }

        // Check if the rectangle fits in the current column
        if (yOffset + rect.height > height) {
            std::cerr << "Rectangle does not fit in the square." << std::endl;
            return std::vector<std::shared_ptr<Box>>();
        }

        // Update the rectangle's position in the original vector
        rect.x = xOffset;
        rect.y = yOffset;

        // Draw the rectangle on the image (for visualization)
        cv::rectangle(squareMat, rect, cv::Scalar(0, 0, 255), 2);

        // Update offsets and row height
        xOffset += rect.width;
        currentRowHeight = std::max(currentRowHeight, rect.height);

        results.push_back(boxes.front());
        boxes.pop_front();
    }

    return results;
}
//=============================================================================
cv::Mat createCollage(cv::Mat& originalMat, std::vector<std::shared_ptr<Box>>& boxes, int width, int height) {
    cv::Mat collage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    for (auto& box : boxes) {
        cv::Mat srcROI = originalMat(box->originalRect);

        if (box->resized) {
            cv::resize(srcROI, srcROI, cv::Size(box->rect.width, box->rect.height));
        }

        srcROI.copyTo(collage(box->rect));
    }

    return collage;
}
//=============================================================================
void reverseCollage(cv::Mat& originalMat,
    const cv::Mat& collageMat,
    std::vector<std::shared_ptr<Box>>& results,
    std::vector<cv::Rect>& detectedBoxes
)
{
    for (const auto& detectedBox : detectedBoxes) {
        for (auto& box : results) {
            auto& collageBox = box->rect;
            if (isOverlapSignificant(collageBox, detectedBox)) {
                cv::Rect& originalBox = box->originalRect;

                if (!box->resized) {
                    int diff_x = detectedBox.x - collageBox.x;
                    if (diff_x < 0) {
                        diff_x = 0;
                    }
                    int diff_y = detectedBox.y - collageBox.y;
                    if (diff_y < 0) {
                        diff_y = 0;
                    }
                    originalBox.x += diff_x;
                    originalBox.y += diff_y;
                    originalBox.width = std::min(originalBox.width, detectedBox.width);
                    originalBox.height = std::min(originalBox.height, detectedBox.height);
                }
                //TODO: check for optimization if resized = else{...}

                cv::rectangle(originalMat, originalBox, cv::Scalar(0, 255, 0), 2);
                break;
            }
        }
    }
}
//=============================================================================