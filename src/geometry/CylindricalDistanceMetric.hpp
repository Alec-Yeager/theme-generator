#pragma once
#include "DistanceMetric.hpp"

class CylindricalDistanceMetric : public DistanceMetric {
public:
    float calculate(const float &ax, const float &ay, const float &az, const float &bx, const float &by,
                    const float &bz) const override;
};