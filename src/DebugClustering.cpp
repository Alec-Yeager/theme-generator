#include "DebugClustering.hpp"
#include <spdlog/spdlog.h>

std::vector<cv::Vec3b> DebugClustering::clusterValues(const cv::Mat &image, const size_t n_clusters) {
    SPDLOG_DEBUG("n clusters: {}", n_clusters);
    auto cluster_means = std::vector<cv::Vec3b>();
    cluster_means.reserve(n_clusters);

    for (size_t i = 0; i < n_clusters; ++i) {
        size_t px_idx = 0;
        auto pxval = image.at<cv::Vec3b>(px_idx % (image.cols), (px_idx / image.rows));
        while ((std::find_if(cluster_means.begin(), cluster_means.end(),
                             [&pxval](const cv::Vec3b &mean) { return cv::norm(pxval, mean, cv::NORM_L2) < 40; }) !=
                cluster_means.end()) &&
               px_idx < image.cols * image.rows) {
            pxval = image.at<cv::Vec3b>(px_idx / (image.cols), (px_idx % image.cols));
            ++px_idx;
        }
        cluster_means.push_back(pxval);
    }
    return cluster_means;
}