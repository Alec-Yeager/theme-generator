#include "PaletteHandler.hpp"
#include <spdlog/spdlog.h>

PaletteHandler::PaletteHandler() {}

Palette PaletteHandler::generateDarkPalette(std::vector<cv::Vec3b> colors) {
    std::vector<int> lightvalues;
    lightvalues.reserve(colors.size());
    std::transform(colors.begin(), colors.end(), std::back_inserter(lightvalues), [](cv::Vec3b in) {
        cv::Mat bgrMat(1, 1, CV_8UC3, in);
        cv::Mat hls;
        cv::cvtColor(bgrMat, hls, cv::COLOR_BGR2HLS);
        SPDLOG_DEBUG("RGB: ({}, {}, {})\nLightness: {}", in[2], in[1], in[0], hls.at<cv::Vec3b>(0, 0)[1]);
        return hls.at<cv::Vec3b>(0, 0)[1];
    });

    auto min_it = std::min_element(lightvalues.begin(), lightvalues.end());
    int min_index = std::distance(lightvalues.begin(), min_it);
    cv::Vec3b bg = colors[min_index];
    colors.erase(colors.begin() + min_index);

    auto max_it = std::max_element(lightvalues.begin(), lightvalues.end());
    int max_index = std::distance(lightvalues.begin(), max_it);
    cv::Vec3b fg = colors[max_index];
    colors.erase(colors.begin() + max_index);

    return Palette(fg, bg, colors);
}
