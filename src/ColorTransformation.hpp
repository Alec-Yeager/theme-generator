#pragma once
#include <opencv2/opencv.hpp>

// Strategy class for color transformations.
class ColorTransformation {
public:
    virtual ~ColorTransformation() = default;
    virtual cv::Mat transformImage(const cv::Mat &image) = 0;
    virtual cv::Vec3b transformPoint(const cv::Vec3b) = 0;
    virtual cv::Vec3b transformPointBack(const cv::Vec3b) = 0;
};