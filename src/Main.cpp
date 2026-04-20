#include "ImageHandler.hpp"
#include "clustering/ClusterReplay.hpp"
#include "clustering/ClusteringAlgorithm.hpp"
#include "clustering/DebugClustering.hpp"
#include "clustering/KMeansClustering.hpp"
#include "clustering/OptimizedKMeansClustering.hpp"
#include "coloring/BGRtoHSLuvTransformation.hpp"
#include "coloring/ColorTransformation.hpp"
#include "coloring/Palette.hpp"
#include "coloring/PaletteHandler.hpp"
#include "export/ColorExporter.hpp"
#include "export/ExporterRegistry.hpp"
#include "export/JsonExporter.hpp"
#include "visualization/ClusteringVisualizer.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <vtkVersion.h>

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::debug);
#endif

    SPDLOG_DEBUG("VTK version: {}", vtkVersion::GetVTKVersion());
    SPDLOG_DEBUG("Currently in: {}", fs::current_path().string());
    std::vector<ImageHandler> imageHandlers{};
    auto debug_clustering_alg = std::make_shared<DebugClustering>();
    auto hamerly_k_means = std::make_shared<KMeansClusteringHamerly>();
    auto hamerly_k_means_optimized = std::make_shared<OptimizedKMeansClusteringHamerly>();

    ExporterRegistry exporter_registry;
    exporter_registry.registerExporter(std::make_unique<JsonExporter>());

    for (const auto &entry : fs::directory_iterator("../test/images")) {
        if (fs::is_regular_file(entry)) {
            SPDLOG_DEBUG("Found file: {}.", entry.path().string());
            imageHandlers.emplace_back(entry.path(), debug_clustering_alg);
        }
    }

    // vtk_tester.run();

    // This is a bit stupid, prolly a better way to handle the empty handlers
    for (auto &ih : imageHandlers) {
        // continue;
        std::string name = ih.path().filename().string();
        SPDLOG_DEBUG("Displaying image: {}.", name);
        if (ih.image().empty()) {
            SPDLOG_ERROR("Failed to load {}", name);
            continue;
        }

        // auto &val = ih.image().at<cv::Vec3b>(0, 0);
        //  SPDLOG_DEBUG("RGB: ({},{},{})", val[0], val[1], val[2]);

        auto hsluv_transform = std::make_shared<BGRtoHSLuvTransformation>();
        auto replay = ClusterReplay(ClusterTracker());
        ClusteringVisualizer vizualizer_test{ih.image(), replay};

        ih.setClusterAlg(hamerly_k_means);
        ih.calculateClusterMeans(8);
        ih.displayMeansInImage();

        Palette p = PaletteHandler::generateDarkPalette(ih.means());
        exporter_registry.runExporters(p, std::vector<std::string>{"JSON"});

        // ih.setTransforms(std::vector<std::shared_ptr<ColorTransformation>>{hsluv_transform});
        // ih.setClusterAlg(hamerly_k_means);
        // ih.calculateClusterMeans(8);
        // ih.displayMeansInImage();

        // cv::namedWindow(name);
        // cv::imshow(name, ih.paletteImage());
        // cv::waitKey(1);
        SPDLOG_DEBUG("About to call vizualizer_test.run()");
        vizualizer_test.run();
        SPDLOG_DEBUG("Returned from vizualizer_test.run()");
        // cv::destroyAllWindows();
    }

    // while (!((cv::waitKey(1) & 0xEFFFFF) == 27))
    //     ;
    // cv::destroyAllWindows();

    return 0;
}
