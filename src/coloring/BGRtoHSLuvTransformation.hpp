#pragma once
#include "ColorTransformation.hpp"
#include "DistanceMetric.hpp"
#include <opencv2/opencv.hpp>

// This is my favorite colorspace
class BGRtoHSLuvTransformation : public ColorTransformation {
public:
    cv::Mat transformImage(const cv::Mat &image) override;
    cv::Vec3b transformPoint(const cv::Vec3b &color) override;
    cv::Vec3b transformPointBack(const cv::Vec3b &color) override;
    std::unique_ptr<DistanceMetric> getDistanceMetric() override;
};
