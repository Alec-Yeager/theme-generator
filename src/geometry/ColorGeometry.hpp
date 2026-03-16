#pragma once
#include "ColorAccumulator.hpp"
#include "DistanceMetric.hpp"
#include <opencv2/opencv.hpp>

enum GeometryType { EUCLIDEAN, CYLINDRICAL };

class ColorGeometry {
public:
    virtual ~ColorGeometry() = default;

    virtual std::unique_ptr<ColorAccumulator> getAccumulator() const = 0;
    virtual std::unique_ptr<DistanceMetric> getDistanceMetric() const = 0;
    virtual cv::Vec3b getXYZ(cv::Vec3b colorPoint) const = 0;
    virtual GeometryType getGeometryType() const = 0;
};