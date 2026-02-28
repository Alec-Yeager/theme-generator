#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::debug);
#endif

    SPDLOG_DEBUG("hmmmmm");
    std::vector<fs::path> filepaths{};
    filepaths.reserve(4);
    for (const auto &entry : fs::directory_iterator("../test/images"))
    {
        if (fs::is_regular_file(entry))
        {
            SPDLOG_DEBUG("Found file: {}.", entry.path().string());
            filepaths.push_back(entry.path());
        }
    }

    std::vector<cv::Mat> images{};
    images.reserve(filepaths.size());
    for (const auto &filepath : filepaths)
    {
        SPDLOG_DEBUG("Loading file: {}.", filepath.string());
        images.emplace_back(cv::imread(filepath));
    }

    // This is a bit stupid.
    for (int i = 0; i < images.size(); i++)
    {
        std::string name = filepaths[i].filename().string();
        SPDLOG_DEBUG("Displaying image: {}.", name);
        if (images[i].empty())
        {
            SPDLOG_ERROR("Failed to load {}", name);
            continue;
        }

        cv::namedWindow(name);
        cv::imshow(name, images[i]);
    }

    while (!((cv::waitKey(1) & 0xEFFFFF) == 27))
        ;
    cv::destroyAllWindows();

    return 0;
}
