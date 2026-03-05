#pragma once
#include "ColorAccumulator.hpp"

class EuclideanColorAccumulator : public ColorAccumulator {
public:
    EuclideanColorAccumulator() : ColorAccumulator() {};

    void add(cv::Vec3b val, bool include_count = true);
    void subtract(cv::Vec3b val, bool include_count = true);
    cv::Vec3b mean(size_t n = 0);
    void reset();
    size_t getCount() { return count_; };

private:
    std::array<int64_t, 3> linear_sum_{0, 0, 0};
    size_t count_ = 0;
};
