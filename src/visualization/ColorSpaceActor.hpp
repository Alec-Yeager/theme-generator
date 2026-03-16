#pragma once
#include "../coloring/ColorTransformation.hpp"
#include "../geometry/ColorGeometry.hpp"
#include <opencv2/opencv.hpp>
#include <vtk/vtkActor.h>
#include <vtk/vtkNew.h>
#include <vtk/vtkObject.h>
#include <vtk/vtkPoints.h>
#include <vtk/vtkPolyData.h>
#include <vtk/vtkPolyDataMapper.h>
#include <vtk/vtkProperty.h>
#include <vtk/vtkRenderer.h>
#include <vtk/vtkSphereSource.h>

class ColorSpaceActor {
public:
    ColorSpaceActor(cv::Mat image, ColorTransformation *transform);
    ~ColorSpaceActor();

    vtkSmartPointer<vtkActor> getActor() { return actor_; }
    vtkSmartPointer<vtkActor> getAxes() { return axes_; }

private:
    vtkNew<vtkPoints> points_;
    vtkNew<vtkPolyData> polydata_;
    vtkNew<vtkPolyDataMapper> mapper_;
    vtkNew<vtkActor> actor_;
    vtkSmartPointer<vtkActor> axes_;
    vtkNew<vtkProperty> prop_;

    void createAxes(ColorTransformation *transform);
    vtkSmartPointer<vtkActor> createCubeAxes(ColorTransformation *transform);
    vtkSmartPointer<vtkActor> createCylinderAxes(ColorTransformation *transform);
};