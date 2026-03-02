#include "CylindricalColorAccumulator.hpp"

void CylindricalColorAccumulator::add(cv::Vec3b val, bool include_count) {

    angular_sum_[0] += std::sin(static_cast<double>(val[0]) * (M_PI / 180.0));
    angular_sum_[1] += std::cos(static_cast<double>(val[0]) * (M_PI / 180.0));
    linear_sum_[0] += val[1];
    linear_sum_[1] += val[2];
    count_++;
}

void CylindricalColorAccumulator::subtract(cv::Vec3b val, bool include_count) {
    angular_sum_[0] -= std::sin(static_cast<double>(val[0]) * (M_PI / 180.0));
    angular_sum_[1] -= std::cos(static_cast<double>(val[0]) * (M_PI / 180.0));
    linear_sum_[0] -= val[1];
    linear_sum_[1] -= val[2];
    count_--;
}

cv::Vec3b CylindricalColorAccumulator::mean(size_t n) {
    if (n == 0) {
        n = count_;
    }

    auto mean_radians = (std::atan2(angular_sum_[0], angular_sum_[1]));
    auto mean_degrees = mean_radians * (180.0 / M_PI);

    return cv::Vec3b{static_cast<u_char>(std::round(mean_degrees)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[0]) / n)),
                     static_cast<u_char>(std::round(static_cast<double>(linear_sum_[1]) / n))};
}

void CylindricalColorAccumulator::reset() {
    angular_sum_ = {0.0, 0.0};
    linear_sum_ = {0, 0};
}
