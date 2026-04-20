#include "ColorSpaceActor.hpp"
#include "../coloring/ColorTransformation.hpp"
#include "../geometry/ColorGeometry.hpp"
#include "../geometry/EuclideanColorGeometry.hpp"
#include <spdlog/spdlog.h>

#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>

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
        // cv does things in BGR, so here we swap to RGB in both space and color
        auto point_id = points_->InsertNextPoint(xyz[2], xyz[1], xyz[0]);
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
    auto actor = vtkSmartPointer<vtkActor>::New();
    vtkNew<vtkPolyData> axes;
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> lines;
    vtkNew<vtkFloatArray> scalars;

    scalars->SetNumberOfComponents(4);

    // Main Grid
    std::array<std::array<double, 3>, 8> point_array = {{{0, 0, 0},
                                                         {255, 0, 0},
                                                         {255, 255, 0},
                                                         {0, 255, 0},
                                                         {0, 0, 255},
                                                         {255, 0, 255},
                                                         {255, 255, 255},
                                                         {0, 255, 255}}};

    std::array<std::array<vtkIdType, 2>, 12> line_array = {
        {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 5}, {5, 6}, {6, 7}, {7, 4}}};

    for (auto i = 0ul; i < point_array.size(); ++i) {
        points->InsertPoint(i, point_array[i].data());
        scalars->InsertTuple4(i, point_array[i][0] / 255.0, point_array[i][1] / 255.0, point_array[i][2] / 255.0, 1.0);
    }

    for (auto &&i : line_array) {
        lines->InsertNextCell(vtkIdType(i.size()), i.data());
    }

    auto point_idx = points->GetNumberOfPoints();
    // Now we do the less opaque middle grid on the edge:
    for (int i = 1; i <= AXIS_SEGMENTS; ++i) {
        points->InsertNextPoint(0, 0, i * 255.0 / AXIS_SEGMENTS);
        points->InsertNextPoint(255, 0, i * 255.0 / AXIS_SEGMENTS);
        points->InsertNextPoint(255, 255, i * 255.0 / AXIS_SEGMENTS);
        points->InsertNextPoint(0, 255, i * 255.0 / AXIS_SEGMENTS);

        scalars->InsertNextTuple4(0, 0, i * 1.0 / AXIS_SEGMENTS, 0.2);
        scalars->InsertNextTuple4(1.0, 0, i * 1.0 / AXIS_SEGMENTS, 0.2);
        scalars->InsertNextTuple4(1.0, 1.0, i * 1.0 / AXIS_SEGMENTS, 0.2);
        scalars->InsertNextTuple4(0, 1.0, i * 1.0 / AXIS_SEGMENTS, 0.2);

        std::array<std::array<vtkIdType, 2>, 4> edge_lines = {{{point_idx, point_idx + 1},
                                                               {point_idx + 1, point_idx + 2},
                                                               {point_idx + 2, point_idx + 3},
                                                               {point_idx + 3, point_idx}}};
        for (auto &&i : edge_lines) {
            lines->InsertNextCell(vtkIdType(i.size()), i.data());
        }

        point_idx += 4;

        points->InsertNextPoint(0, i * 255.0 / AXIS_SEGMENTS, 0);
        points->InsertNextPoint(255, i * 255.0 / AXIS_SEGMENTS, 0);
        points->InsertNextPoint(255, i * 255.0 / AXIS_SEGMENTS, 255);
        points->InsertNextPoint(0, i * 255.0 / AXIS_SEGMENTS, 255);

        scalars->InsertNextTuple4(0, i * 1.0 / AXIS_SEGMENTS, 0, 0.2);
        scalars->InsertNextTuple4(1.0, i * 1.0 / AXIS_SEGMENTS, 0, 0.2);
        scalars->InsertNextTuple4(1.0, i * 1.0 / AXIS_SEGMENTS, 1.0, 0.2);
        scalars->InsertNextTuple4(0, i * 1.0 / AXIS_SEGMENTS, 1.0, 0.2);

        edge_lines = {{{point_idx, point_idx + 1},
                       {point_idx + 1, point_idx + 2},
                       {point_idx + 2, point_idx + 3},
                       {point_idx + 3, point_idx}}};
        for (auto &&i : edge_lines) {
            lines->InsertNextCell(vtkIdType(i.size()), i.data());
        }

        point_idx += 4;

        points->InsertNextPoint(i * 255.0 / AXIS_SEGMENTS, 0, 0);
        points->InsertNextPoint(i * 255.0 / AXIS_SEGMENTS, 255, 0);
        points->InsertNextPoint(i * 255.0 / AXIS_SEGMENTS, 255, 255);
        points->InsertNextPoint(i * 255.0 / AXIS_SEGMENTS, 0, 255);

        scalars->InsertNextTuple4(i * 1.0 / AXIS_SEGMENTS, 0, 0, 0.2);
        scalars->InsertNextTuple4(i * 1.0 / AXIS_SEGMENTS, 1.0, 0, 0.2);
        scalars->InsertNextTuple4(i * 1.0 / AXIS_SEGMENTS, 1.0, 1.0, 0.2);
        scalars->InsertNextTuple4(i * 1.0 / AXIS_SEGMENTS, 0, 1.0, 0.2);

        edge_lines = {{{point_idx, point_idx + 1},
                       {point_idx + 1, point_idx + 2},
                       {point_idx + 2, point_idx + 3},
                       {point_idx + 3, point_idx}}};
        for (auto &&i : edge_lines) {
            lines->InsertNextCell(vtkIdType(i.size()), i.data());
        }

        point_idx += 4;
    }

    axes->SetPoints(points);
    axes->SetLines(lines);
    axes->GetPointData()->SetScalars(scalars);

    vtkNew<vtkPolyDataMapper> axes_mapper;
    axes_mapper->SetInputData(axes);
    axes_mapper->ScalarVisibilityOn();
    axes_mapper->SetScalarModeToUsePointData();
    axes_mapper->SetColorModeToDirectScalars();
    axes_mapper->SetScalarRange(axes->GetScalarRange());

    actor->SetMapper(axes_mapper);

    return actor;
}

vtkSmartPointer<vtkActor> ColorSpaceActor::createCylinderAxes(ColorTransformation *transform) {
    return vtkSmartPointer<vtkActor>::New();
}