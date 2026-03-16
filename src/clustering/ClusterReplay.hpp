#pragma once
#include "ClusterStage.hpp"
#include "ClusterTracker.hpp"
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <vector>

// A class for getting concrete stages out of a cluster tracker.
class ClusterReplay {
public:
    ClusterReplay(ClusterTracker tracker)
        : tracker_(tracker), current_cluster_assignments_(tracker.getInitialClusterAssignments()) {};
    ~ClusterReplay() = default;

    // Sets to stage 0
    void resetStage();
    // number of stages
    size_t stageCount() const;
    // current stage number
    size_t currentStage() const;
    // increment stage by 1
    void increment();
    // decrement stage by 1
    void decrement();
    // set stage (uses inc/dec under the hood)
    void setStage(size_t stage);

    const std::vector<cv::Vec3b> &getClusterMeans() const {
        return tracker_.getStages()[current_stage_].getClusterMeans();
    }
    const std::vector<size_t> &getAssignments() const;

private:
    ClusterTracker tracker_;
    std::vector<size_t> current_cluster_assignments_;
    size_t current_stage_{0};
};