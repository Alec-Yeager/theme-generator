#pragma once
#include "../coloring/ColorTransformation.hpp"
#include "../geometry/ColorGeometry.hpp"
#include <opencv2/opencv.hpp>
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkObject.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

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