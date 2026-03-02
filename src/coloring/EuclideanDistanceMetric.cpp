#include "EuclideanDistanceMetric.hpp"
#include <opencv2/opencv.hpp>

float EuclideanDistanceMetric::calculate(const float &ax, const float &ay, const float &az, const float &bx,
                                         const float &by, const float &bz) const {
    float dx = (ax - bx) / 255.0;
    float dy = (ay - by) / 255.0;
    float dz = (az - bz) / 255.0;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}