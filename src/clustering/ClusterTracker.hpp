#pragma once
#include "ClusterStage.hpp"
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <vector>

// Class for accumulating cluster information for visualization
class ClusterTracker {
public:
    ClusterTracker(std::vector<cv::Vec3b> initial_cluster_means, std::vector<size_t> initial_cluster_assignments)
        : initial_cluster_means_(initial_cluster_means), initial_cluster_assignments_(initial_cluster_assignments) {}
    ~ClusterTracker() = default;

    const std::vector<cv::Vec3b> &getCurrentClusterMeans() const {
        return stages_.size() > 0 ? stages_.back().getClusterMeans() : initial_cluster_means_;
    }

    const std::vector<cv::Vec3b> &getInitialClusterMeans() const { return initial_cluster_means_; }

    const std::vector<size_t> &getCurrentClusterAssignments() const { return initial_cluster_assignments_; };
    const std::vector<size_t> &getInitialClusterAssignments() const { return current_cluster_assignments_; };

    const std::vector<ClusterStage> &getStages() const { return stages_; };

    // This is a copy. I don't care that much for now.
    void addStage(ClusterStage stage);

private:
    std::vector<cv::Vec3b> initial_cluster_means_;
    std::vector<size_t> initial_cluster_assignments_;
    // Don't need the current cluster means since the stages store those raw and not as deltas.
    std::vector<size_t> current_cluster_assignments_;
    std::vector<ClusterStage> stages_;
};