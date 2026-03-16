#pragma once

#include "ColorGeometry.hpp"
#include "EuclideanColorAccumulator.hpp"
#include "EuclideanDistanceMetric.hpp"

class EuclideanColorGeometry : public ColorGeometry {
public:
    EuclideanColorGeometry() : ColorGeometry() {};
    std::unique_ptr<ColorAccumulator> getAccumulator() const override {
        return std::make_unique<EuclideanColorAccumulator>();
    };
    std::unique_ptr<DistanceMetric> getDistanceMetric() const override {
        return std::make_unique<EuclideanDistanceMetric>();
    };
    cv::Vec3b getXYZ(cv::Vec3b colorPoint) const override { return colorPoint; };
    GeometryType getGeometryType() const { return GeometryType::EUCLIDEAN; };
};