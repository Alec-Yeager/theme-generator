#pragma once
#include <format>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

// meme code. Where should I even put this? lol
inline std::string bgrToHex(cv::Vec3b color) {
    std::stringstream ss;
    std::string r = std::format("{:02X}", color[2]);
    std::string g = std::format("{:02X}", color[1]);
    std::string b = std::format("{:02X}", color[0]);
    ss << "#" << r << g << b;
    return ss.str();
}

class Palette {
public:
    Palette(cv::Vec3b foreground, cv::Vec3b background, std::vector<cv::Vec3b> colors);
    ~Palette() = default;

    const cv::Vec3b &foreground() const { return foreground_; };
    const cv::Vec3b &background() const { return background_; };
    const std::vector<cv::Vec3b> &colors() const { return colors_; };

private:
    cv::Vec3b foreground_;
    cv::Vec3b background_;
    std::vector<cv::Vec3b> colors_;
};