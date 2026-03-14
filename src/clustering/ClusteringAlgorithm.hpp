#pragma once
#include "../geometry/ColorGeometry.hpp"
#include "ClusterTracker.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

class ClusteringAlgorithm {
public:
    ClusteringAlgorithm() {};
    virtual ~ClusteringAlgorithm() = default;

    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                 const ColorGeometry &geometry) {
        auto tracker = ClusterTracker();
        return clusterValues(image, n_clusters, geometry, tracker);
    }

    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                 const ColorGeometry &geometry, ClusterTracker &tracker) = 0;

protected:
};