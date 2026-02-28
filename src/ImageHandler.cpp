#include "ImageHandler.hpp"
#include <spdlog/spdlog.h>

ImageHandler::ImageHandler(const std::filesystem::path &path, std::shared_ptr<ClusteringAlgorithm> algorithm,
                           std::vector<std::shared_ptr<ColorTransformation>> transforms)
    : filepath_(path), image_(cv::imread(path)), cluster_alg_(algorithm), transforms_(transforms) {}

std::vector<cv::Vec3b> ImageHandler::calculateClusterMeans(size_t k) {
    transformed_image_ = image_;
    for (auto stage : transforms_) {
        transformed_image_ = stage->transformImage(transformed_image_);
    }
    auto means = cluster_alg_->clusterValues(transformed_image_, k);
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

    // Arbitrary.
    int buffer_width = im_width / 5;
    // Obviously may have rounding difficulties but I'll just pad later.
    int block_height = im_height / n_means;

    cv::Mat colorColumn{};
    colorColumn.reserve(im_height);
    for (auto &mean : means) {
        colorColumn.push_back(cv::Mat(block_height, buffer_width, image_.type(), mean));
    }

    // Pad the bottom.
    if (colorColumn.rows != image_.rows) {
        colorColumn.push_back(
            cv::Mat(image_.rows - colorColumn.rows, buffer_width, image_.type(), means[means.size() - 1]));
    }
    cv::Mat concatenated_image{};
    cv::hconcat(image_, colorColumn, concatenated_image);
    image_ = concatenated_image;
}
