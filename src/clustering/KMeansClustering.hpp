#pragma once
#include "../geometry/ColorGeometry.hpp"
#include "../geometry/DistanceMetric.hpp"
#include "ClusteringAlgorithm.hpp"

class KMeansClustering : public ClusteringAlgorithm {
public:
    KMeansClustering() : ClusteringAlgorithm() {};
    virtual std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                 const ColorGeometry &geometry) override = 0;

protected:
    virtual std::vector<cv::Vec3b> calcKppSeedCentroids(const cv::Mat &image, const size_t n_clusters,
                                                        const ColorGeometry &geometry);
    std::vector<cv::Vec3b> randomSeedCentroids(const cv::Mat &image, const size_t n_clusters);
};

// I'm just going to store these all together. I know it's not idiomatic lol.
class KMeansClusteringHamerly : public KMeansClustering {
public:
    KMeansClusteringHamerly() : KMeansClustering() {};
    std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                         const ColorGeometry &geometry) override;
};

class KMeansClusteringNaive : public KMeansClustering {
public:
    KMeansClusteringNaive() : KMeansClustering() {};
    std::vector<cv::Vec3b> clusterValues(const cv::Mat &image, const size_t n_clusters,
                                         const ColorGeometry &geometry) override;
};
