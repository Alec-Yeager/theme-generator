#include "ClusteringVisualizer.hpp"

ClusteringVisualizer::ClusteringVisualizer(cv::Mat image, ColorTransformation *transform, ClusterReplay &replay)
    : cs_actor_(image, transform), replay_(replay) {}

ClusteringVisualizer::~ClusteringVisualizer() {}

void ClusteringVisualizer::run() {

    iren_->SetInteractorStyle(camera_interactor_style_);

    // Add the actor constructed by cs_actor_
    ren_->SetBackground(1.0, 1.0, 1.0);
    ren_->AddActor(cs_actor_.getActor());
    win_->AddRenderer(ren_);
    iren_->SetRenderWindow(win_);

    iren_->Render();
    iren_->Start();

    return;
}
