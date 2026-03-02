#include "KMeansClustering.hpp"
#include "../geometry/ColorGeometry.hpp"
#include <chrono>
#include <random>
#include <spdlog/spdlog.h>

// This assumes a 3 component image. Might update later to allow for other sizes.
std::vector<cv::Vec3b> KMeansClustering::calcKppSeedCentroids(const cv::Mat &image, const size_t n_clusters,
                                                              const ColorGeometry &geometry) {
    std::vector<cv::Vec3b> centroids{};
    auto metric = geometry.getDistanceMetric();

    if (n_clusters == 0) {
        return centroids;
    }

    unsigned seed = std::random_device{}();
    // debug
    // seed = 21223;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<uint64_t> distrib(0, image.total());

    int first_index = distrib(generator);
    centroids.push_back(image.at<cv::Vec3b>(first_index / image.cols, first_index % image.cols));

    // choose n centroids
    while (centroids.size() < n_clusters) {
        SPDLOG_DEBUG("Finding seed {}", centroids.size());
        std::vector<float> sq_dists{};
        sq_dists.reserve(image.total());

        // compute squared dist (for us its ^4 lol) to nearest centroid
        for (auto it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
            int64_t fx = (*it)[0];
            int64_t fy = (*it)[1];
            int64_t fz = (*it)[2];

            float min_dist = metric->calculate(fx, fy, fz, centroids[0]);
            for (int j = 1; j < centroids.size(); j++) {

                float dist = metric->calculate(fx, fy, fz, centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
            sq_dists.emplace_back(min_dist * min_dist);
        }

        // create a new centroid based on the dist squared probability
        double total = std::accumulate(sq_dists.begin(), sq_dists.end(), 0.0);
        auto float_distrib = std::uniform_real_distribution<double>(0.0, total);
        double threshold = float_distrib(generator);
        double cumulative = 0;
        // SPDLOG_DEBUG("Total: {}, Threshold: {}", total, threshold);
        for (size_t i = 0; i < sq_dists.size(); i++) {
            cumulative += sq_dists[i];
            if (cumulative >= threshold) {
                centroids.push_back(image.at<cv::Vec3b>(i / image.cols, i % image.cols));
                break;
            }
        }
        SPDLOG_DEBUG("Seed {} chosen: ({}, {}, {})", centroids.size() - 1, centroids[centroids.size() - 1][0],
                     centroids[centroids.size() - 1][1], centroids[centroids.size() - 1][2]);
    }

    return centroids;
}

std::vector<cv::Vec3b> KMeansClustering::randomSeedCentroids(const cv::Mat &image, const size_t n_clusters) {
    std::vector<cv::Vec3b> centroids{};

    if (n_clusters == 0) {
        return centroids;
    }

    unsigned seed = std::random_device{}();
    // debug
    // seed = 21232;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<uint64_t> distrib(0, image.total());

    std::sample(image.begin<cv::Vec3b>(), image.end<cv::Vec3b>(), std::back_inserter(centroids), n_clusters, generator);

    return centroids;
}

// Thanks Hamerly, this DOES require 2 clusters min tho
std::vector<cv::Vec3b> KMeansClusteringHamerly::clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                              const ColorGeometry &geometry) {
    if (n_clusters == 0) {
        return std::vector<cv::Vec3b>();
    }

    auto metric = geometry.getDistanceMetric();

    auto start = std::chrono::steady_clock::now();

    // Initialize our values.
    // Cluster related vectors
    std::vector<cv::Vec3b> centroids = calcKppSeedCentroids(image, n_clusters, geometry);
    // admittedly this assumes an RGB image. Might update later for BW images... or 4 comp images...
    std::vector<std::array<int64_t, 3>> cluster_vector_sums = std::vector<std::array<int64_t, 3>>(n_clusters);
    std::vector<int32_t> n_points_in_clusters = std::vector<int32_t>(n_clusters);
    std::vector<float> distances_last_moved = std::vector<float>(n_clusters);
    std::vector<float> nearest_centroid_distances = std::vector<float>(n_clusters, INT32_MAX);

    // Point related vectors
    std::vector<size_t> assigned_clusters = std::vector<size_t>(image.total());
    std::vector<float> distance_upper_bounds = std::vector<float>(image.total());
    std::vector<float> distance_lower_bounds = std::vector<float>(image.total());

    // this operation is called "POINT-ALL-CTRS" in the literature.
    // A little silly to pass in both but eh
    auto pointAllCenters = [&](const int32_t &fx, const int32_t &fy, const int32_t &fz, const size_t &idx) {
        float min_dist = metric->calculate(fx, fy, fz, centroids[0]);
        // impossibly large to make sure it gets overwritten later
        float second_min_dist = INT32_MAX;
        size_t min_idx = 0;

        for (int j = 1; j < centroids.size(); j++) {
            float dist = metric->calculate(fx, fy, fz, centroids[j]);
            if (dist < min_dist) {
                second_min_dist = min_dist;
                min_dist = dist;
                min_idx = j;
            } else {
                second_min_dist = std::min(second_min_dist, dist);
            }
        }
        assigned_clusters[idx] = min_idx;
        distance_upper_bounds[idx] = min_dist;
        distance_lower_bounds[idx] = second_min_dist;
    };

    // Initialize the point-based values.
    size_t idx = 0;
    for (auto it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
        int32_t fx = (*it)[0];
        int32_t fy = (*it)[1];
        int32_t fz = (*it)[2];
        pointAllCenters(fx, fy, fz, idx);
        n_points_in_clusters[assigned_clusters[idx]] += 1;
        auto &cluster_vector_sum = cluster_vector_sums[assigned_clusters[idx]];
        cluster_vector_sum[0] += fx;
        cluster_vector_sum[1] += fy;
        cluster_vector_sum[2] += fz;
        ++idx;
    }

    bool converged = false;
    int iteration = 0;

    while (!converged) {
        SPDLOG_DEBUG("ITERATION: {}, centroids:", iteration);
        for (auto cent : centroids) {
            SPDLOG_DEBUG("\t({}, {}, {})", cent[0], cent[1], cent[2]);
        }

        // j index to match literature.
        // Reset the nearest distances.
        nearest_centroid_distances = std::vector<float>(n_clusters, FLT_MAX);
        // For each centroid, find the distance to the nearest other centroid.
        for (size_t j = 0; j < centroids.size(); ++j) {
            for (size_t jp = j + 1; jp < centroids.size(); ++jp) {

                auto &cent1 = centroids[j];
                auto &cent2 = centroids[jp];

                float centroid_dist = metric->calculate(cent1, cent2);

                nearest_centroid_distances[j] = std::min(nearest_centroid_distances[j], centroid_dist);
                nearest_centroid_distances[jp] = std::min(nearest_centroid_distances[jp], centroid_dist);
            }
        }

        // Do our work on the points.
        idx = 0;
        for (auto it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
            auto current_assigned_cluster = assigned_clusters[idx];
            // Note that I'm using squared distances here.
            // I may actually need to change this 2 constant, need to investigate.
            int32_t triangle_ineq =
                std::max(nearest_centroid_distances[current_assigned_cluster] / 2, distance_lower_bounds[idx]);
            // First bounds test.
            if (distance_upper_bounds[idx] > triangle_ineq) {
                // Tighten bounds and try again.
                int32_t fx = (*it)[0];
                int32_t fy = (*it)[1];
                int32_t fz = (*it)[2];

                auto &centroid = centroids[current_assigned_cluster];

                distance_upper_bounds[idx] = metric->calculate(fx, fy, fz, centroid);

                // Check again.
                if (distance_upper_bounds[idx] > triangle_ineq) {
                    // recalc point.
                    pointAllCenters(fx, fy, fz, idx);
                    // did our cluster change?
                    if (current_assigned_cluster != assigned_clusters[idx]) {
                        n_points_in_clusters[current_assigned_cluster] -= 1;
                        n_points_in_clusters[assigned_clusters[idx]] += 1;

                        auto &old_cluster_vector_sum = cluster_vector_sums[current_assigned_cluster];
                        auto &new_cluster_vector_sum = cluster_vector_sums[assigned_clusters[idx]];

                        old_cluster_vector_sum[0] -= fx;
                        old_cluster_vector_sum[1] -= fy;
                        old_cluster_vector_sum[2] -= fz;

                        new_cluster_vector_sum[0] += fx;
                        new_cluster_vector_sum[1] += fy;
                        new_cluster_vector_sum[2] += fz;
                    }
                }
            }
            ++idx;
        }

        // Now we do the move centers operation.
        for (size_t j = 0; j < centroids.size(); ++j) {

            auto old_centroid = centroids[j];
            auto &centroid_ref = centroids[j];
            auto vector_sum = cluster_vector_sums[j];
            auto points_in_cluster = n_points_in_clusters[j];

            // Empty centroid problem.
            if (points_in_cluster == 0) {
                // pick a random pixel as the new centroid
                int random_idx = rand() % image.total();
                centroids[j] = image.at<cv::Vec3b>(random_idx / image.cols, random_idx % image.cols);
                centroid_ref = centroids[j];
            } else {
                centroid_ref[0] = vector_sum[0] / points_in_cluster;
                centroid_ref[1] = vector_sum[1] / points_in_cluster;
                centroid_ref[2] = vector_sum[2] / points_in_cluster;
            }

            distances_last_moved[j] = metric->calculate(centroid_ref, old_centroid);
        }

        if (std::accumulate(distances_last_moved.begin(), distances_last_moved.end(), 0.0f) == 0.0f) {
            converged = true;
            break;
        }

        // And we update the bounds, finally.
        size_t greatest_moved_center = 0;
        size_t second_greatest_moved_center = 0;
        float greatest_dist = distances_last_moved[0];
        float second_greatest_dist = -FLT_MAX;
        for (size_t j = 1; j < centroids.size(); ++j) {
            float dist = distances_last_moved[j];
            if (dist > greatest_dist) {
                second_greatest_dist = greatest_dist;
                greatest_dist = dist;
                second_greatest_moved_center = greatest_moved_center;
                greatest_moved_center = j;
            } else if (dist > second_greatest_dist) {
                second_greatest_dist = dist;
                second_greatest_moved_center = j;
            }
        }

        for (size_t i = 0; i < image.total(); ++i) {
            distance_upper_bounds[i] += distances_last_moved[assigned_clusters[i]];
            if (greatest_moved_center == assigned_clusters[i]) {
                distance_lower_bounds[i] =
                    std::max(0.0f, distance_lower_bounds[i] - distances_last_moved[second_greatest_moved_center]);
            } else {
                distance_lower_bounds[i] =
                    std::max(0.0f, distance_lower_bounds[i] - distances_last_moved[greatest_moved_center]);
            }
        }
        iteration++;
    }

    SPDLOG_DEBUG("FINAL ITERATION: {}, centroids:", iteration + 1);
    for (auto cent : centroids) {
        SPDLOG_DEBUG("\t({}, {}, {})", cent[0], cent[1], cent[2]);
    }

    auto end = std::chrono::steady_clock::now();

    // Calculate the duration and cast it to the desired unit (e.g., milliseconds)
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    SPDLOG_DEBUG("Time taken for k means: {} ms", duration.count());

    return centroids;
}

// STILL GOTTA FIX THIS LATER LOL
std::vector<cv::Vec3b> KMeansClusteringNaive::clusterValues(const cv::Mat &image, const size_t n_clusters,
                                                            const ColorGeometry &geometry) {
    if (n_clusters == 0) {
        return std::vector<cv::Vec3b>();
    }

    auto metric = geometry.getDistanceMetric();
    // Create initial points.
    auto start = std::chrono::steady_clock::now();
    auto centroids = calcKppSeedCentroids(image, n_clusters, geometry);
    // auto centroids = std::vector<cv::Vec3b>(n_clusters);
    // unsigned seed = std::random_device{}();
    // std::mt19937 generator(seed);
    // std::sample(image.begin<cv::Vec3b>(), image.end<cv::Vec3b>(), centroids.begin(), n_clusters, generator);

    bool converged = false;

    // Tons of data copying here. I'm sure there are better ways.

    int iteration = 0;

    while (!converged) {
        SPDLOG_DEBUG("ITERATION: {}, centroids:", iteration);
        for (auto cent : centroids) {
            SPDLOG_DEBUG("\t({}, {}, {})", cent[0], cent[1], cent[2]);
        }
        std::vector<cv::Vec3f> cluster_sums(n_clusters);
        std::vector<size_t> cluster_counts(n_clusters);
        for (auto it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
            // auto point = *it;
            int32_t fx = (*it)[0];
            int32_t fy = (*it)[1];
            int32_t fz = (*it)[2];

            int32_t dx = (fx - centroids[0][0]);
            int32_t dy = (fy - centroids[0][1]);
            int32_t dz = (fz - centroids[0][2]);

            size_t closest_index = 0;
            int32_t min_distance = dx * dx + dy * dy + dz * dz;
            for (size_t j = 1; j < centroids.size(); ++j) {
                dx = (fx - centroids[j][0]);
                dy = (fy - centroids[j][1]);
                dz = (fz - centroids[j][2]);
                int32_t dist = dx * dx;
                // partial distance just to get rid of some of the meme distances
                if (dist > min_distance) {
                    continue;
                } else {
                    dist += dy * dy + dz * dz;
                }
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_index = j;
                }
            }
            cluster_sums[closest_index][0] += fx;
            cluster_sums[closest_index][1] += fy;
            cluster_sums[closest_index][2] += fz;
            cluster_counts[closest_index] += 1;
        }
        std::vector<cv::Vec3b> new_centroids{};
        for (size_t i = 0; i < n_clusters; ++i) {
            cluster_sums[i] /= static_cast<float>(cluster_counts[i]);
            new_centroids.push_back(static_cast<cv::Vec3b>(cluster_sums[i]));
        }

        if (new_centroids == centroids) {
            converged = true;
        } else {
            centroids = new_centroids;
        }
        ++iteration;
    }

    auto end = std::chrono::steady_clock::now();

    // Calculate the duration and cast it to the desired unit (e.g., milliseconds)
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    SPDLOG_DEBUG("Time taken for k means: {} ms", duration.count());

    return centroids;
}
