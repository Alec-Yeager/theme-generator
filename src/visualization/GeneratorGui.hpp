#pragma once
#include "../ImageHandler.hpp"
#include "ClusteringVisualizer.hpp"
#include <GLFW/glfw3.h>
#include <filesystem>

// Class for controlling the main gui. Handles the GLFW and whatnot.
class GeneratorGui {
public:
    GeneratorGui();
    ~GeneratorGui();

    void run();
    void loadImage(std::filesystem::path image_path);

private:
    GLFWwindow *window_;
    std::vector<ClusteringVisualizer> visualizers_{};
    std::vector<ImageHandler> imageHandlers_{};

    void initGLFW();
    void initImGUI();
};
