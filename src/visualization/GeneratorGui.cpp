#include "GeneratorGui.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// One of the below includes some gl stuff that conflict with GLAD. Keep it up there.
#include "../clustering/KMeansClustering.hpp"
#include "ClusteringVisualizer.hpp"
#include "ColorSpaceActor.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GeneratorGui::GeneratorGui() {}

GeneratorGui::~GeneratorGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void GeneratorGui::run() {
    initGLFW();
    initImGUI();

    while (!glfwWindowShouldClose(window_)) {

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (ClusteringVisualizer &vis : visualizers_) {
            vis.draw();
        }

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // No idea if this is needed:
        // Update and Render additional Platform Windows
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window_);
    }

    glfwDestroyWindow(window_);
    window_ = nullptr;

    return;
}

void GeneratorGui::loadImage(std::filesystem::path image_path) {
    auto hamerly_k_means = std::make_shared<KMeansClusteringHamerly>();
    imageHandlers_.emplace_back(image_path, hamerly_k_means);
}

static void glfw_error_callback(int error, const char *description) {
    SPDLOG_ERROR("Glfw Error {}: {}\n", error, description);
}

void GeneratorGui::initGLFW() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        SPDLOG_ERROR("Failed to init glfw");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    window_ = glfwCreateWindow(1280, 720, "Dear ImGui VTKViewer Example", NULL, NULL);
    if (window_ == NULL) {
        SPDLOG_ERROR("Failed to create glfw Window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window_);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // Enable vsync, do I need this?
}

void GeneratorGui::initImGUI() {
    const char *glsl_version = "#version 130";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows'
    ImGui::StyleColorsDark();
    SPDLOG_DEBUG("Initializing ImGUI for new window.");
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}