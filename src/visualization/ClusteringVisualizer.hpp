#pragma once
#include "../clustering/ClusterReplay.hpp"
#include "ColorSpaceActor.hpp"
#include <vtk/vtkActor.h>
#include <vtk/vtkCallbackCommand.h>
#include <vtk/vtkEventData.h>
#include <vtk/vtkInteractorStyleTrackballCamera.h>
#include <vtk/vtkNew.h>
#include <vtk/vtkObject.h>
#include <vtk/vtkPolyDataMapper.h>
#include <vtk/vtkProperty.h>
#include <vtk/vtkRenderWindow.h>
#include <vtk/vtkRenderWindowInteractor.h>
#include <vtk/vtkRenderer.h>
#include <vtk/vtkSphereSource.h>

class ClusteringVisualizer {
public:
    // Maybe the cluster replay should be owned by this. Same with the transform.
    ClusteringVisualizer(cv::Mat image, ColorTransformation *transform, ClusterReplay &replay);
    ClusteringVisualizer(cv::Mat image, ClusterReplay &replay) : ClusteringVisualizer(image, nullptr, replay) {}
    ~ClusteringVisualizer();

    void run();

private:
    ColorSpaceActor cs_actor_;
    ClusterReplay replay_;
    vtkNew<vtkRenderer> ren_;
    vtkNew<vtkRenderWindow> win_;
    vtkNew<vtkRenderWindowInteractor> iren_;
    vtkNew<vtkInteractorStyleTrackballCamera> camera_interactor_style_;
};