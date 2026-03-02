#include "FirstPolarDistanceMetric.hpp"

float FirstPolarDistanceMetric::calculate(const float &ax, const float &ay, const float &az, const float &bx,
                                          const float &by, const float &bz) const {
    // Assumes the polar coord comes in as 180 max (opencv convention)
    float dx = std::abs(ax - bx);
    dx = std::min(dx, 180 - dx) / 180.0;
    float dy = (ay - by) / 255.0;
    float dz = (az - bz) / 255.0;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}