#pragma once
#include "Palette.hpp"
#include <opencv2/opencv.hpp>

class PaletteHandler {

public:
    PaletteHandler(/* args */);
    ~PaletteHandler() = default;

    static Palette generateDarkPalette(std::vector<cv::Vec3b> colors);

private:
    /* data */
};
