#pragma once
#include "ClusteringAlgorithm.hpp"
#include "ColorTransformation.hpp"

#include <filesystem>
#include <opencv2/opencv.hpp>

class ImageHandler {
public:
    ImageHandler(const std::filesystem::path &path, std::shared_ptr<ClusteringAlgorithm> algorithm,
                 std::vector<std::shared_ptr<ColorTransformation>> transforms =
                     std::vector<std::shared_ptr<ColorTransformation>>());
    ~ImageHandler() = default;

    std::filesystem::path path() { return filepath_; };
    const cv::Mat &image() { return image_; };

    std::vector<cv::Vec3b> calculateClusterMeans(size_t k);
    void displayMeansInImage(std::vector<cv::Vec3b> means);
    void displayMeansInImage() { displayMeansInImage(means_); };

private:
    std::shared_ptr<ClusteringAlgorithm> cluster_alg_;
    std::vector<std::shared_ptr<ColorTransformation>> transforms_;
    std::filesystem::path filepath_;
    cv::Mat image_;
    // Maybe don't actually need to store this but eh
    cv::Mat transformed_image_;
    std::vector<cv::Vec3b> means_{};
};