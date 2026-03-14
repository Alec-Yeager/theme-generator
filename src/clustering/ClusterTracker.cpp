#include "ClusterTracker.hpp"

void ClusterTracker::setInitialMeansAndAssignments(std::vector<cv::Vec3b> initial_cluster_means,
                                                   std::vector<size_t> initial_cluster_assignments) {
    initial_cluster_means_ = initial_cluster_means;
    initial_cluster_assignments_ = initial_cluster_assignments;
    current_cluster_assignments_ = initial_cluster_assignments;
}

void ClusterTracker::addStage(ClusterStage stage) {
    stages_.push_back(stage);
    for (auto delta : stage.getAssignmentDeltas()) {
        current_cluster_assignments_[delta.index] = delta.to;
    }
}