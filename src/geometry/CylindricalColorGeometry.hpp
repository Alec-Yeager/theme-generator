#pragma once

#include "ColorGeometry.hpp"
#include "CylindricalColorAccumulator.hpp"
#include "CylindricalDistanceMetric.hpp"

class CylindricalColorGeometry : public ColorGeometry {
public:
    CylindricalColorGeometry() : ColorGeometry() {};
    std::unique_ptr<ColorAccumulator> getAccumulator() const override {
        return std::make_unique<CylindricalColorAccumulator>();
    };
    std::unique_ptr<DistanceMetric> getDistanceMetric() const override {
        return std::make_unique<CylindricalDistanceMetric>();
    };
};