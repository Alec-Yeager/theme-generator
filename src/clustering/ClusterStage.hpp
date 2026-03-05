#pragma once
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <vector>

// Represents a single difference between clustering stages (what point moved from what cluster to where)
struct AssignmentDelta {
    AssignmentDelta(size_t index, size_t from, size_t to) : index(index), from(from), to(to) {}
    size_t index;
    size_t from;
    size_t to;
};

// Represents a single stage of clustering iteration
class ClusterStage {
public:
    ClusterStage(std::vector<cv::Vec3b> cluster_means) : cluster_means_(cluster_means) {}
    ~ClusterStage() = default;
    const std::vector<cv::Vec3b> &getClusterMeans() const { return cluster_means_; }

    void addDelta(AssignmentDelta delta) { assignemnt_deltas_.push_back(delta); };
    void addDelta(size_t index, size_t from, size_t to) { assignemnt_deltas_.emplace_back(index, from, to); };

private:
    std::vector<cv::Vec3b> cluster_means_;
    std::vector<AssignmentDelta> assignemnt_deltas_;
};