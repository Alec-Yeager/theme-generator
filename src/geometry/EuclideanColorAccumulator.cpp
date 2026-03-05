#include "EuclideanColorAccumulator.hpp"
#include <spdlog/spdlog.h>

void EuclideanColorAccumulator::add(cv::Vec3b val, bool include_count) {
    linear_sum_[0] += val[0];
    linear_sum_[1] += val[1];
    linear_sum_[2] += val[2];
    if (include_count)
        count_++;
}

void EuclideanColorAccumulator::subtract(cv::Vec3b val, bool include_count) {
    linear_sum_[0] -= val[0];
    linear_sum_[1] -= val[1];
    linear_sum_[2] -= val[2];
    if (include_count)
        count_--;
}

cv::Vec3b EuclideanColorAccumulator::mean(size_t n) {
    if (n == 0) {
        n = count_;
    }

    SPDLOG_DEBUG("average b, g, r: ({}, {}, {})", std::round(static_cast<double>(linear_sum_[0]) / n),
                 std::round(static_cast<double>(linear_sum_[1]) / n),
                 std::round(static_cast<double>(linear_sum_[2]) / n));

    return cv::Vec3b{static_cast<u_char>(std::round(static_cast<double>(linear_sum_[0]) / n)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[1]) / n)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[2]) / n))};
}

void EuclideanColorAccumulator::reset() {
    linear_sum_ = {0, 0, 0};
    count_ = 0;
}