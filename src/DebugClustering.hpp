#pragma once
#include "ClusteringAlgorithm.hpp"

class DebugClustering : public ClusteringAlgorithm {
public:
    DebugClustering() : ClusteringAlgorithm() {};
    std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters) override;
};