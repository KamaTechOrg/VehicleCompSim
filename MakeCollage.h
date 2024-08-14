#ifndef MAKE_COLLAGE_H
#define MAKE_COLLAGE_H

#include "open_cv.h"
#include <vector>
#include <deque>
#include <unordered_map>
#include <memory> // Add this for shared_ptr

struct Box
{
    cv::Rect rect;
    cv::Rect originalRect;
    bool resized = false;
    Box(const cv::Rect& original) : rect(original), originalRect(original) {}
};

std::deque<std::shared_ptr<Box>> getBoxesDQ(const std::vector<cv::Rect>& diff_boxes);
void resizeBoxToLimit(const std::shared_ptr<Box>& box, int limit);
std::vector<cv::Rect> getDetectedBoxes(cv::Mat& collage);
void resizeOverLimitBoxes(std::deque<std::shared_ptr<Box>>& boxes);
void drawFrameOnDetectedBox(const std::shared_ptr<Box>& box, const cv::Rect& detectedBox, cv::Mat& originalMat);
void upgradeSizesToYolos(const std::shared_ptr<Box>& box, const cv::Rect& detectedBox);

bool isOverlapSignificant(const cv::Rect& rect1, const cv::Rect& rect2, double threshold = 0.8);
cv::Mat optimizeCollageGreedy(cv::Mat& originalMat, std::vector<cv::Rect>& diff_boxes);
std::vector<std::shared_ptr<Box>> arrangeRectanglesInSquare_FFT(std::deque<std::shared_ptr<Box>>& boxes, int width, int height);
cv::Mat createCollage(cv::Mat& originalMat, std::vector<std::shared_ptr<Box>>& results, int width, int height);
void reverseCollage(cv::Mat& originalMat,
    const cv::Mat& collageMat,
    std::vector<std::shared_ptr<Box>>& results,
    std::vector<cv::Rect>& detectedBoxes,
    cv::Mat& copyMat
);

#endif // MAKE_COLLAGE_H
