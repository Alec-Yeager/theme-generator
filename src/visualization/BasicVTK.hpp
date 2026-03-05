#pragma once
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

class BasicVTK {
public:
    BasicVTK();
    ~BasicVTK();

    void run();
    void exit();

private:
    vtkNew<vtkSphereSource> sphere_source_;
    vtkNew<vtkPolyDataMapper> mapper_;
    vtkNew<vtkActor> actor_;
    vtkNew<vtkProperty> prop_;
    vtkNew<vtkRenderer> ren_;
    vtkNew<vtkRenderWindow> win_;
    vtkNew<vtkRenderWindowInteractor> iren_;
    vtkNew<vtkInteractorStyleTrackballCamera> camera_interactor_style_;
};