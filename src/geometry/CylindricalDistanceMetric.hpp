#pragma once
#include "DistanceMetric.hpp"
#include <opencv4/opencv2/opencv.hpp>

class CylindricalDistanceMetric : public DistanceMetric {
public:
    float calculate(const float &ax, const float &ay, const float &az, const float &bx, const float &by,
                    const float &bz) const override;
};