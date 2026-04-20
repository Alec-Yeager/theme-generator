#include <glad/glad.h>

#include <GLFW/glfw3.h>

// One of the below includes some gl stuff that conflict with GLAD. Keep it up there.
#include "ClusteringVisualizer.hpp"
#include "ColorSpaceActor.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ClusteringVisualizer::ClusteringVisualizer(cv::Mat image, ColorTransformation *transform, ClusterReplay &replay)
    : cs_actor_(std::make_unique<ColorSpaceActor>(image, transform)), replay_(replay) {

    viewer_ = VtkViewer();
    viewer_.init();
    viewer_.getRenderer()->SetBackground(1.0, 1.0, 1.0);
    viewer_.getRenderer()->AddActor(cs_actor_->getActor());
}

ClusteringVisualizer::~ClusteringVisualizer() {
    // viewer_.getRenderWindow();
    cs_actor_.release();
}

void ClusteringVisualizer::run() {}

void ClusteringVisualizer::draw() {
    ImGui::Begin("Vtk Viewer 1", nullptr, VtkViewer::NoScrollFlags());
    ImGui::Text("Imgui is displaying text frfr");
    if (ImGui::Button("<", ImVec2(20, 20))) {
    }
    ImGui::SameLine();
    if (ImGui::Button(">", ImVec2(20, 20))) {
    }

    static bool show_axes = false;
    if (ImGui::Checkbox("Show Axes", &show_axes)) {
        if (show_axes) {
            viewer_.getRenderer()->AddActor(cs_actor_->getAxes());
        } else {
            viewer_.getRenderer()->RemoveActor(cs_actor_->getAxes());
        }
    }
    viewer_.render(); // default render size = ImGui::GetContentRegionAvail()
    ImGui::End();
}
