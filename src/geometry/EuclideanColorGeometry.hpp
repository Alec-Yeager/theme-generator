#pragma once

#include "ColorGeometry.hpp"
#include "EuclideanColorAccumulator.hpp"
#include "EuclideanDistanceMetric.hpp"

class EuclideanColorGeometry : public ColorGeometry {
public:
    EuclideanColorGeometry() : ColorGeometry() {};
    std::unique_ptr<ColorAccumulator> getAccumulator() override {
        return std::make_unique<EuclideanColorAccumulator>();
    };
    std::unique_ptr<DistanceMetric> getDistanceMetric() override {
        return std::make_unique<EuclideanDistanceMetric>();
    };
};