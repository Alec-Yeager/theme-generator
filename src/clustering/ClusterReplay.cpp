#include "ClusterReplay.hpp"

void ClusterReplay::resetStage() {
    current_stage_ = 0;
    current_cluster_assignments_ = tracker_.getInitialClusterAssignments();
}

size_t ClusterReplay::stageCount() const { return tracker_.getStages().size(); }

size_t ClusterReplay::currentStage() const { return current_stage_; }

void ClusterReplay::increment() {
    if (current_stage_ == stageCount() - 1) {
        SPDLOG_WARN("Trying to set the current stage to {}, max is {}", current_stage_ + 1, stageCount());
        return;
    }
    current_stage_++;
    auto &stage = tracker_.getStages()[current_stage_];
    for (auto delta : stage.getAssignmentDeltas()) {
        current_cluster_assignments_[delta.index] = delta.to;
    }
}

void ClusterReplay::decrement() {
    if (current_stage_ == 0) {
        SPDLOG_WARN("Trying to set the current stage to {}, max is {}", current_stage_ - 1, stageCount());
        return;
    }
    // Note that because stage 0 has no deltas, we need to do this step first, then decrement
    auto &stage = tracker_.getStages()[current_stage_];
    for (auto delta : stage.getAssignmentDeltas()) {
        current_cluster_assignments_[delta.index] = delta.from;
    }
    current_stage_--;
}

void ClusterReplay::setStage(size_t stage) {
    if (stage < 0 || stage >= stageCount()) {
        // Not even bothering to do an error here.
        SPDLOG_WARN("Trying to set the current stage to {}, max is {}", stage, stageCount());
        return;
    }
    // obv need to static cast the size ts
    auto func = (static_cast<int>(stage) - static_cast<int>(current_stage_) > 0)
                    ? std::function<void()>([this]() { increment(); })
                    : std::function<void()>([this]() { decrement(); });
    while (stage != current_stage_) {
        func();
    }
}

const std::vector<size_t> &ClusterReplay::getAssignments() const { return current_cluster_assignments_; }
