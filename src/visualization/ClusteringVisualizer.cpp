#include <glad/glad.h>

#include <GLFW/glfw3.h>

// One of the below includes some gl stuff that conflict with GLAD. Keep it up there.
#include "ClusteringVisualizer.hpp"
#include "ColorSpaceActor.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ClusteringVisualizer::ClusteringVisualizer(cv::Mat image, ColorTransformation *transform, ClusterReplay &replay)
    : cs_actor_(std::make_unique<ColorSpaceActor>(image, transform)), replay_(replay) {
    camera_interactor_style_->SetCurrentStyleToTrackballCamera();
}

ClusteringVisualizer::~ClusteringVisualizer() {
    // viewer_.getRenderWindow();
    cs_actor_.release();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

static void glfw_error_callback(int error, const char *description) {
    SPDLOG_ERROR("Glfw Error {}: {}\n", error, description);
}

void ClusteringVisualizer::run() {

    SPDLOG_DEBUG("Initializing GLAD");
    initGLFW();
    initImGUI();

    viewer_ = VtkViewer();
    viewer_.init();

    // iren_->SetInteractorStyle(camera_interactor_style_);
    viewer_.setInteractorStyle(camera_interactor_style_);
    viewer_.setRenderer(ren_);

    // Add the actor constructed by cs_actor_
    ren_->SetBackground(1.0, 0.0, 1.0);
    // ren_->AddActor(cs_actor_->getActor());
    // win_->AddRenderer(ren_);
    // iren_->SetRenderWindow(win_);

    while (!glfwWindowShouldClose(window_)) {

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::SetNextWindowSize(ImVec2(360, 240), ImGuiCond_FirstUseEver);
        ImGui::Begin("Vtk Viewer 1", nullptr, VtkViewer::NoScrollFlags());
        ImGui::Text("Imgui is displaying text frfr");
        viewer_.render(); // default render size = ImGui::GetContentRegionAvail()
        ImGui::End();

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
    // iren_->Render();
    // iren_->Start();

    return;
}

void ClusteringVisualizer::initGLFW() {
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

void ClusteringVisualizer::initImGUI() {
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