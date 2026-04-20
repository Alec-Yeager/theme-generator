#include "Palette.hpp"

Palette::Palette(cv::Vec3b foreground, cv::Vec3b background, std::vector<cv::Vec3b> colors)
    : foreground_(foreground), background_(background), colors_(colors) {}