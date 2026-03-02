#pragma once
#include "ColorAccumulator.hpp"

class CylindricalColorAccumulator : public ColorAccumulator {
public:
    CylindricalColorAccumulator() : ColorAccumulator() {}

    void add(cv::Vec3b val, bool include_count = true);
    void subtract(cv::Vec3b val, bool include_count = true);
    cv::Vec3b mean(size_t n = 0);
    void reset();

private:
    std::array<uint64_t, 4> sum_;
    size_t count_;
};
