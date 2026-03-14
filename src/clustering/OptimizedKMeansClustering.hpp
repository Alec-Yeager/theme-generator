#pragma once
#include "../geometry/ColorGeometry.hpp"
#include "KMeansClustering.hpp"

// These are the optimized beasts. They don't use the geometry or tracker. eh. Not the cleanest, but whatever.

// Does not allow for geometry or tracking.
class OptimizedKMeansClustering : public KMeansClustering {
public:
    OptimizedKMeansClustering() : KMeansClustering() {};
    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                 const ColorGeometry &geometry, ClusterTracker &tracker) override {
        return clusterValues(image, n_clusters);
    };
    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters) = 0;

protected:
    std::vector<cv::Vec3b> calcKppSeedCentroids(const cv::Mat &image, const size_t n_clusters);
};

class OptimizedKMeansClusteringHamerly : public OptimizedKMeansClustering {
public:
    OptimizedKMeansClusteringHamerly() : OptimizedKMeansClustering() {};
    std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters) override;
};

class OptimizedKMeansClusteringNaive : public OptimizedKMeansClustering {
public:
    OptimizedKMeansClusteringNaive() : OptimizedKMeansClustering() {};
    std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters) override;
};