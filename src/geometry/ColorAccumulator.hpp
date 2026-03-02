#pragma once
#include <array>
#include <opencv2/opencv.hpp>
#include <stddef.h>

class ColorAccumulator {
public:
    virtual ~ColorAccumulator() = default;

    virtual void add(cv::Vec3b val, bool include_count = true) = 0;
    virtual void subtract(cv::Vec3b val, bool include_count = true) = 0;
    virtual cv::Vec3b mean(size_t n = 0) = 0;
    virtual void reset() = 0;
    virtual size_t getCount() = 0;
};