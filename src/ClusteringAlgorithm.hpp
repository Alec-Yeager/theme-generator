#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class ClusteringAlgorithm {
public:
    ClusteringAlgorithm() {};
    virtual ~ClusteringAlgorithm() = default;

    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters) = 0;

protected:
};