#pragma once
#include "ColorAccumulator.hpp"
#include "DistanceMetric.hpp"

class ColorGeometry {
public:
    virtual ~ColorGeometry() = default;

    virtual std::unique_ptr<ColorAccumulator> getAccumulator() = 0;
    virtual std::unique_ptr<DistanceMetric> getDistanceMetric() = 0;
};