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
    const std::vector<AssignmentDelta> &getAssignmentDeltas() const { return assignment_deltas_; }

    void addDelta(AssignmentDelta delta) { assignment_deltas_.push_back(delta); };
    void addDelta(size_t index, size_t from, size_t to) { assignment_deltas_.emplace_back(index, from, to); };

private:
    std::vector<cv::Vec3b> cluster_means_;
    std::vector<AssignmentDelta> assignment_deltas_;
};

// It's important to recall that this is the centroid, calculated in the previous iteration,
// and how the assignments changed due to that centroid.
// This means the first stage centroid IS the initial condition, with no deltas.