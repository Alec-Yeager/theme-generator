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
    cv::Vec3b getXYZ(cv::Vec3b colorPoint) const override {
        return cv::Vec3b(colorPoint[1] * std::cos(static_cast<double>(colorPoint[0]) * 2 * (M_PI / 180.0)),
                         colorPoint[1] * std::sin(static_cast<double>(colorPoint[0]) * 2 * (M_PI / 180.0)),
                         colorPoint[2]);
    };

    GeometryType getGeometryType() const { return GeometryType::CYLINDRICAL; };
};