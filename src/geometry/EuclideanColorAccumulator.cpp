#include "EuclideanColorAccumulator.hpp"

void EuclideanColorAccumulator::add(cv::Vec3b val, bool include_count) {
    linear_sum_[0] += val[0];
    linear_sum_[1] += val[1];
    linear_sum_[2] += val[2];
    count_++;
}

void EuclideanColorAccumulator::subtract(cv::Vec3b val, bool include_count) {
    linear_sum_[0] -= val[0];
    linear_sum_[1] -= val[1];
    linear_sum_[2] -= val[2];
    count_--;
}

cv::Vec3b EuclideanColorAccumulator::mean(size_t n) {
    if (n == 0) {
        n = count_;
    }
    return cv::Vec3b{static_cast<u_char>(std::round(static_cast<double>(linear_sum_[0]) / n)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[1]) / n)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[2]) / n))};
}

void EuclideanColorAccumulator::reset() { linear_sum_ = {0, 0, 0}; }