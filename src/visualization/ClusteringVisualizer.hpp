#pragma once
#include "../clustering/ClusterReplay.hpp"
#include "ColorSpaceActor.hpp"
#include "VtkViewer.h"
#include <GLFW/glfw3.h>
#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkEventData.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkNew.h>
#include <vtkObject.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

class ClusteringVisualizer {
public:
    // Maybe the cluster replay should be owned by this. Same with the transform.
    ClusteringVisualizer(cv::Mat image, ColorTransformation *transform, ClusterReplay &replay);
    ClusteringVisualizer(cv::Mat image, ClusterReplay &replay) : ClusteringVisualizer(image, nullptr, replay) {}
    ~ClusteringVisualizer();

    void run();

private:
    std::unique_ptr<ColorSpaceActor> cs_actor_;
    ClusterReplay replay_;
    vtkNew<vtkRenderer> ren_;
    VtkViewer viewer_;
    // vtkNew<vtkRenderWindow> win_;
    // vtkNew<vtkRenderWindowInteractor> iren_;
    GLFWwindow *window_;
    vtkNew<vtkInteractorStyleSwitch> camera_interactor_style_;

    void initGLFW();
    void initImGUI();
};