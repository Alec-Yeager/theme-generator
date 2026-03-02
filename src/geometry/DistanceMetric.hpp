#pragma once
#include <opencv2/opencv.hpp>

class DistanceMetric {
public:
    virtual ~DistanceMetric() = default;

    // Returns a float 0-1, whwere 1 is the maxiumum possible distance allowed by the color space.
    virtual float calculate(const cv::Vec3b &a, const cv::Vec3b &b) const {
        return calculate(a[0], a[1], a[2], b[0], b[1], b[2]);
    };

    virtual float calculate(const float &ax, const float &ay, const float &az, const float &bx, const float &by,
                            const float &bz) const = 0;

    template <typename T> float calculate(const T &ax, const T &ay, const T &az, const cv::Vec3b &b) const {
        return calculate(static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az),
                         static_cast<float>(b[0]), static_cast<float>(b[1]), static_cast<float>(b[2]));
    }
    template <typename T>
    float calculate(const T &ax, const T &ay, const T &az, const T &bx, const T &by, const T &bz) const {
        return calculate(static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az), static_cast<float>(bx),
                         static_cast<float>(by), static_cast<float>(bz));
    }
};