#include "ClusteringAlgorithm.hpp"
#include "ColorTransformation.hpp"
#include "DebugClustering.hpp"
#include "ImageHandler.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::debug);
#endif

    SPDLOG_DEBUG("Currently in: {}", fs::current_path().string());
    std::vector<ImageHandler> imageHandlers{};
    auto clustering_alg = std::make_shared<DebugClustering>();

    for (const auto &entry : fs::directory_iterator("../test/images")) {
        if (fs::is_regular_file(entry)) {
            SPDLOG_DEBUG("Found file: {}.", entry.path().string());
            imageHandlers.emplace_back(entry.path(), clustering_alg);
        }
    }

    // This is a bit stupid, prolly a better way to handle the empty handlers
    for (auto &ih : imageHandlers) {
        std::string name = ih.path().filename().string();
        SPDLOG_DEBUG("Displaying image: {}.", name);
        if (ih.image().empty()) {
            SPDLOG_ERROR("Failed to load {}", name);
            continue;
        }

        // auto &val = ih.image().at<cv::Vec3b>(0, 0);
        //  SPDLOG_DEBUG("RGB: ({},{},{})", val[0], val[1], val[2]);
        auto vals = ih.calculateClusterMeans(6);
        ih.displayMeansInImage();

        cv::namedWindow(name);
        cv::imshow(name, ih.image());
    }

    while (!((cv::waitKey(1) & 0xEFFFFF) == 27))
        ;
    cv::destroyAllWindows();

    return 0;
}
