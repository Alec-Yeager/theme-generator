#include "ImageHandler.hpp"
#include "clustering/ClusterReplay.hpp"
#include "clustering/ClusteringAlgorithm.hpp"
#include "clustering/DebugClustering.hpp"
#include "clustering/KMeansClustering.hpp"
#include "clustering/OptimizedKMeansClustering.hpp"
#include "coloring/BGRtoHSLuvTransformation.hpp"
#include "coloring/ColorTransformation.hpp"
#include "visualization/BasicVTK.hpp"

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
    auto debug_clustering_alg = std::make_shared<DebugClustering>();
    auto hamerly_k_means = std::make_shared<KMeansClusteringHamerly>();
    auto hamerly_k_means_optimized = std::make_shared<OptimizedKMeansClusteringHamerly>();
    auto naive_k_means = std::make_shared<KMeansClusteringNaive>();

    for (const auto &entry : fs::directory_iterator("../test/images")) {
        if (fs::is_regular_file(entry)) {
            SPDLOG_DEBUG("Found file: {}.", entry.path().string());
            imageHandlers.emplace_back(entry.path(), debug_clustering_alg);
        }
    }

    BasicVTK vtk_tester{};
    vtk_tester.run();

    // This is a bit stupid, prolly a better way to handle the empty handlers
    for (auto &ih : imageHandlers) {
        continue;
        std::string name = ih.path().filename().string();
        SPDLOG_DEBUG("Displaying image: {}.", name);
        if (ih.image().empty()) {
            SPDLOG_ERROR("Failed to load {}", name);
            continue;
        }

        // auto &val = ih.image().at<cv::Vec3b>(0, 0);
        //  SPDLOG_DEBUG("RGB: ({},{},{})", val[0], val[1], val[2]);

        ih.setClusterAlg(hamerly_k_means_optimized);
        ih.calculateClusterMeans(8);
        ih.displayMeansInImage();
        // ih.setTransforms(
        //     std::vector<std::shared_ptr<ColorTransformation>>{std::make_shared<BGRtoHSLuvTransformation>()});
        // ih.setClusterAlg(hamerly_k_means);
        // ih.calculateClusterMeans(8);
        // ih.displayMeansInImage();

        cv::namedWindow(name);
        cv::imshow(name, ih.paletteImage());
    }

    // while (!((cv::waitKey(1) & 0xEFFFFF) == 27))
    //     ;
    // cv::destroyAllWindows();

    return 0;
}
