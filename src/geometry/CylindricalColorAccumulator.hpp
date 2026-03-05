#pragma once
#include "ColorAccumulator.hpp"

class CylindricalColorAccumulator : public ColorAccumulator {
public:
    CylindricalColorAccumulator() : ColorAccumulator() {};

    void add(cv::Vec3b val, bool include_count = true);
    void subtract(cv::Vec3b val, bool include_count = true);
    cv::Vec3b mean(size_t n = 0);
    void reset();
    size_t getCount() { return count_; };

private:
    // While it might look like this would skew the angle,
    // that would actually require one of these init values to be nonzero.
    // Recall, these are the sin and cos of the angle, not the angle itself.
    // (sin(theta), cos(theta)) = (0,0) is impossible, so this is safe as its nonreflective of a starting angle
    std::array<double, 2> angular_sum_{0.0, 0.0};
    std::array<int64_t, 2> linear_sum_{0, 0};
    size_t count_ = 0;
};
