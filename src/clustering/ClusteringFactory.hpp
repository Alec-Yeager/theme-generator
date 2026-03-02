#pragma once
#include "../coloring/DistanceMetric.hpp"
#include "KMeansClustering.hpp"
#include "OptimizedKMeansClustering.hpp"

// I gave up on this idea. It's annoying, and I don't want to store the metric in the clustering alg.
// the clustering alg uses a metric as part of the clustering, it isn't intrinsic to the alg.
class KMeansClusteringFactory {
public:
    std::unique_ptr<KMeansClustering> create(std::unique_ptr<DistanceMetric> metric = nullptr) {
        if (metric == nullptr) {
            return std::make_unique<OptimizedKMeansClusteringHamerly>();
        } else {
            return std::make_unique<KMeansClusteringHamerly>();
        }
    }
};