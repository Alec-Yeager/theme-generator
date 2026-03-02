#include "ImageHandler.hpp"
#include "geometry/ColorGeometry.hpp"
#include "geometry/EuclideanColorGeometry.hpp"
#include <spdlog/spdlog.h>

ImageHandler::ImageHandler(const std::filesystem::path &path, std::shared_ptr<ClusteringAlgorithm> algorithm,
                           std::vector<std::shared_ptr<ColorTransformation>> transforms)
    : filepath_(path), image_(cv::imread(path)), palette_image_(image_), cluster_alg_(algorithm),
      transforms_(transforms) {}

std::vector<cv::Vec3b> ImageHandler::calculateClusterMeans(size_t k) {
    transformed_image_ = image_;
    for (auto stage : transforms_) {
        transformed_image_ = stage->transformImage(transformed_image_);
    }
    std::unique_ptr<ColorGeometry> geometry;
    if (transforms_.size() == 0) {
        geometry = std::make_unique<EuclideanColorGeometry>();
    } else {
        geometry = transforms_.back()->getColorGeometry();
    }
    auto means = cluster_alg_->clusterValues(transformed_image_, k, *geometry);
    SPDLOG_DEBUG("Received {} means successfully.", means.size());
    // Now undo the color transforms to get BGR back (hopefully lol)
    for (auto rit = transforms_.rbegin(); rit != transforms_.rend(); ++rit) {
        for (auto &mean : means) {
            mean = (*rit)->transformPointBack(mean);
        }
    }
    means_ = means;
    return means;
}

void ImageHandler::displayMeansInImage(std::vector<cv::Vec3b> means) {

    auto n_means = means.size();
    SPDLOG_DEBUG("Adding {} means to image.", n_means);
    if (n_means == 0)
        return;
    auto im_width = image_.size().width;
    auto im_height = image_.size().height;

    // Obviously may have rounding difficulties but I'll just pad later.
    int block_height = im_height / n_means;

    // Arbitrary.
    int block_width = std::min(im_width / 5, block_height);

    cv::Mat colorColumn{};
    colorColumn.reserve(im_height);
    for (auto &mean : means) {
        colorColumn.push_back(cv::Mat(block_height, block_width, image_.type(), mean));
    }

    // Pad the bottom.
    if (colorColumn.rows != image_.rows) {
        colorColumn.push_back(
            cv::Mat(image_.rows - colorColumn.rows, block_width, image_.type(), means[means.size() - 1]));
    }
    cv::Mat concatenated_image{};
    cv::hconcat(palette_image_, colorColumn, concatenated_image);
    palette_image_ = concatenated_image;
}
