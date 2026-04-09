#include "ColorSpaceActor.hpp"
#include "../coloring/ColorTransformation.hpp"
#include "../geometry/ColorGeometry.hpp"
#include "../geometry/EuclideanColorGeometry.hpp"
#include <spdlog/spdlog.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>

ColorSpaceActor::ColorSpaceActor(cv::Mat image, ColorTransformation *transform) {
    std::unique_ptr<ColorGeometry> geometry;
    // shallow copy per opencv
    cv::Mat bgr_image = image;

    if (transform) {
        geometry = transform->getColorGeometry();
        bgr_image = transform->transformImageBack(image);
    } else {
        geometry = std::make_unique<EuclideanColorGeometry>();
    }

    createAxes(transform);

    vtkNew<vtkUnsignedCharArray> colors;
    colors->SetNumberOfComponents(3);
    vtkNew<vtkCellArray> verts;

    auto it1 = image.begin<cv::Vec3b>();
    auto it2 = bgr_image.begin<cv::Vec3b>();
    for (; it1 != image.end<cv::Vec3b>() && it2 != bgr_image.end<cv::Vec3b>(); ++it1, ++it2) {
        auto xyz = geometry->getXYZ(*it1);
        auto color = *it2;
        auto point_id = points_->InsertNextPoint(xyz[0], xyz[1], xyz[2]);
        // cv does things in BGR, so here we swap to RGB;
        colors->InsertNextTuple3(color[2], color[1], color[0]);
        verts->InsertNextCell(1, &point_id);
    }

    polydata_->SetPoints(points_);
    polydata_->GetPointData()->SetScalars(colors);
    polydata_->SetVerts(verts);

    // prop_->SetVertexVisibility(true);
    prop_->SetPointSize(3.0);

    mapper_->SetInputData(polydata_);
    actor_->SetMapper(mapper_);
    actor_->SetProperty(prop_);
}

ColorSpaceActor::~ColorSpaceActor() {}

void ColorSpaceActor::createAxes(ColorTransformation *transform) {
    // I want to keep all the VTK logic here, so instead we just switch on the geometry type.
    // TODO : create the axes.
    if (!transform) {
        axes_ = createCubeAxes(transform);
        return;
    }
    switch (transform->getColorGeometry()->getGeometryType()) {
    case GeometryType::EUCLIDEAN:
        axes_ = createCubeAxes(transform);
        break;
    case GeometryType::CYLINDRICAL:
        axes_ = createCylinderAxes(transform);
        break;
    default:
        axes_ = createCubeAxes(transform);
        break;
    }
}

vtkSmartPointer<vtkActor> ColorSpaceActor::createCubeAxes(ColorTransformation *transform) {
    return vtkSmartPointer<vtkActor>::New();
}

vtkSmartPointer<vtkActor> ColorSpaceActor::createCylinderAxes(ColorTransformation *transform) {
    return vtkSmartPointer<vtkActor>::New();
}