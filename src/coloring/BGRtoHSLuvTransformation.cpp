#include "BGRtoHSLuvTransformation.hpp"
#include "../geometry/CylindricalColorGeometry.hpp"
#include "hsluv.h"
#include <spdlog/spdlog.h>

cv::Mat BGRtoHSLuvTransformation::transformImage(const cv::Mat &image) {

    SPDLOG_DEBUG("Transforming image from BGR to HSLuv");
    auto transformed_image = cv::Mat(image.rows, image.cols, image.type());
    auto it1 = image.begin<cv::Vec3b>();
    auto it2 = transformed_image.begin<cv::Vec3b>();
    for (; it1 != image.end<cv::Vec3b>() && it2 != transformed_image.end<cv::Vec3b>(); ++it1, ++it2) {
        *it2 = transformPoint(*it1);
    }
    SPDLOG_DEBUG("Image transformed.");
    return transformed_image;
}

cv::Vec3b BGRtoHSLuvTransformation::transformPoint(const cv::Vec3b &color) {
    double h, s, l;
    rgb2hsluv(static_cast<double>(color[2]) / 255.0, static_cast<double>(color[1]) / 255.0,
              static_cast<double>(color[0]) / 255.0, &h, &s, &l);
    // From 360 to 180
    h /= 2.0;
    // From 100 to 255
    s *= (255.0 / 100.0);
    l *= (255.0 / 100.0);

    // SPDLOG_DEBUG("({}, {}, {}) -> ({}, {}, {})", color[2], color[1], color[0], h, s, l);

    return cv::Vec3b({static_cast<u_char>(h), static_cast<u_char>(s), static_cast<u_char>(l)});
}

cv::Vec3b BGRtoHSLuvTransformation::transformPointBack(const cv::Vec3b &color) {
    double r, g, b;
    hsluv2rgb(static_cast<double>(color[0]) * 2.0, static_cast<double>(color[1]) * (100.0 / 255.0),
              static_cast<double>(color[2]) * (100.0 / 255.0), &r, &g, &b);
    r *= 255;
    g *= 255;
    b *= 255;
    return cv::Vec3b{static_cast<u_char>(b), static_cast<u_char>(g), static_cast<u_char>(r)};
}

std::unique_ptr<ColorGeometry> BGRtoHSLuvTransformation::getColorGeometry() {
    return std::make_unique<CylindricalColorGeometry>();
}
