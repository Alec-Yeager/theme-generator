#include "VtkViewer.h"
#include <spdlog/spdlog.h>

// dear imgui: Renderer for VTK(OpenGL back end)
// - Desktop GL: 2.x 3.x 4.x
// - Embedded GL: ES 2.0 (WebGL 1.0), ES 3.0 (WebGL 2.0)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..) and a renderer binding (OpenGL)

// Implemented features:

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>                       // intptr_t
#else
#include <stdint.h> // intptr_t
#endif

// OpenGL Loader
// This can be replaced with another loader, e.g. glad, which I have done.
#include <glad/glad.h> // glad, initialized with gl3wInit()

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// Including opencv for the debug write
#include <opencv2/opencv.hpp>
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>

void VtkViewer::isCurrentCallbackFn(vtkObject *caller, long unsigned int eventId, void *clientData, void *callData) {
    bool *isCurrent = static_cast<bool *>(callData);
    *isCurrent = true;
}

void VtkViewer::processEvents() {
    if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
        return;
    }

    SPDLOG_DEBUG("Entering event processing.");

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true; // don't drag window when clicking on image.
    ImVec2 viewportPos = ImGui::GetCursorStartPos();

    double xpos = static_cast<double>(io.MousePos[0]) - static_cast<double>(viewportPos.x);
    double ypos = static_cast<double>(io.MousePos[1]) - static_cast<double>(viewportPos.y);
    int ctrl = static_cast<int>(io.KeyCtrl);
    int shift = static_cast<int>(io.KeyShift);
    bool dclick = io.MouseDoubleClicked[0] || io.MouseDoubleClicked[1] || io.MouseDoubleClicked[2];

    interactor->SetEventInformationFlipY(xpos, ypos, ctrl, shift, dclick);

    if (ImGui::IsWindowHovered()) {
        if (io.MouseClicked[ImGuiMouseButton_Left]) {
            interactor->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
        } else if (io.MouseClicked[ImGuiMouseButton_Right]) {
            interactor->InvokeEvent(vtkCommand::RightButtonPressEvent, nullptr);
            ImGui::SetWindowFocus(); // make right-clicks bring window into focus
        } else if (io.MouseWheel > 0) {
            interactor->InvokeEvent(vtkCommand::MouseWheelForwardEvent, nullptr);
        } else if (io.MouseWheel < 0) {
            interactor->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, nullptr);
        }
    }

    if (io.MouseReleased[ImGuiMouseButton_Left]) {
        interactor->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
    } else if (io.MouseReleased[ImGuiMouseButton_Right]) {
        interactor->InvokeEvent(vtkCommand::RightButtonReleaseEvent, nullptr);
    }

    interactor->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
}

VtkViewer::VtkViewer()
    : viewportWidth_(0), viewportHeight_(0), renderWindow(nullptr), interactor(nullptr), interactorStyle(nullptr),
      renderer(nullptr), tex_(0), firstRender_(true) {
    // init();
}

VtkViewer::VtkViewer(const VtkViewer &vtkViewer)
    : viewportWidth_(0), viewportHeight_(0), renderWindow(vtkViewer.renderWindow), interactor(vtkViewer.interactor),
      interactorStyle(vtkViewer.interactorStyle), renderer(vtkViewer.renderer), tex_(vtkViewer.tex_),
      firstRender_(vtkViewer.firstRender_) {}

VtkViewer::VtkViewer(VtkViewer &&vtkViewer) noexcept
    : viewportWidth_(0), viewportHeight_(0), renderWindow(std::move(vtkViewer.renderWindow)),
      interactor(std::move(vtkViewer.interactor)), interactorStyle(std::move(vtkViewer.interactorStyle)),
      renderer(std::move(vtkViewer.renderer)), tex_(vtkViewer.tex_), firstRender_(vtkViewer.firstRender_) {}

VtkViewer::~VtkViewer() {
    renderer = nullptr;
    interactorStyle = nullptr;
    interactor = nullptr;
    renderWindow = nullptr;

    glDeleteTextures(1, &tex_);
}

VtkViewer &VtkViewer::operator=(const VtkViewer &vtkViewer) {
    viewportWidth_ = vtkViewer.viewportWidth_;
    viewportHeight_ = vtkViewer.viewportHeight_;
    renderWindow = vtkViewer.renderWindow;
    interactor = vtkViewer.interactor;
    interactorStyle = vtkViewer.interactorStyle;
    renderer = vtkViewer.renderer;
    tex_ = vtkViewer.tex_;
    firstRender_ = vtkViewer.firstRender_;
    return *this;
}

void VtkViewer::init() {

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->ResetCamera();
    renderer->SetBackground(DEFAULT_BACKGROUND);
    renderer->SetBackgroundAlpha(DEFAULT_ALPHA);

    interactorStyle = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
    interactorStyle->SetDefaultRenderer(renderer);
    interactorStyle->SetCurrentStyleToTrackballCamera();

    interactor = vtkSmartPointer<vtkGenericRenderWindowInteractor>::New();
    interactor->SetInteractorStyle(interactorStyle);
    interactor->EnableRenderOff();

    int viewportSize[2] = {static_cast<int>(viewportWidth_), static_cast<int>(viewportHeight_)};

    SPDLOG_DEBUG("Initializing the OpenGLRenderWindow");
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->SetSize(viewportSize);

    vtkSmartPointer<vtkCallbackCommand> isCurrentCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    isCurrentCallback->SetCallback(&isCurrentCallbackFn);
    renderWindow->AddObserver(vtkCommand::WindowIsCurrentEvent, isCurrentCallback);

    renderWindow->SwapBuffersOn();

    renderWindow->SetOffScreenRendering(true);
    renderWindow->SetFrameBlitModeToNoBlit();

    renderWindow->AddRenderer(renderer);
    renderWindow->SetInteractor(interactor);

    if (!renderer || !interactorStyle || !renderWindow || !interactor) {
        throw VtkViewerError("Couldn't initialize VtkViewer");
    }
}

void VtkViewer::render() { render(ImGui::GetContentRegionAvail()); }
void VtkViewer::render(const ImVec2 size) {
    // SPDLOG_DEBUG("Render Size: ({}, {})", size.x, size.y);
    setViewportSize(size);

    renderWindow->Render();
    renderWindow->WaitForCompletion();

    vtkNew<vtkWindowToImageFilter> wtoi;
    vtkNew<vtkPNGWriter> writer;

    wtoi->SetInput(renderWindow);
    wtoi->Update();

    const char *home = std::getenv("HOME");
    writer->SetFileName("/home/Vozrazhat/Projects/theme-generator/test/outputs/render.png");
    writer->SetInputConnection(wtoi->GetOutputPort());
    writer->Write();

        std::vector<unsigned char> pixels(size.x * size.y * 4);
    glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    cv::Mat rgba_img(size.y, size.x, CV_8UC4, pixels.data());
    cv::Mat bgra_img;
    cv::cvtColor(rgba_img, bgra_img, cv::COLOR_RGBA2BGRA);

    cv::imwrite("/home/ayeager/Projects/theme-generator/test/outputs/tex.png", bgra_img);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("##Viewport", size, true, VtkViewer::NoScrollFlags());
    ImGui::Image(reinterpret_cast<void *>(tex_), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    processEvents();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void VtkViewer::addActor(const vtkSmartPointer<vtkProp> &actor) {
    renderer->AddActor(actor);
    renderer->ResetCamera();
}

void VtkViewer::addActors(const vtkSmartPointer<vtkPropCollection> &actors) {
    actors->InitTraversal();
    vtkProp *actor;
    vtkCollectionSimpleIterator sit;
    for (actors->InitTraversal(sit); (actor = actors->GetNextProp(sit));) {
        renderer->AddActor(actor);
        renderer->ResetCamera();
    }
}

void VtkViewer::removeActor(const vtkSmartPointer<vtkProp> &actor) { renderer->RemoveActor(actor); }

void VtkViewer::setViewportSize(const ImVec2 newSize) {
    if (newSize.x <= 0 || newSize.y <= 0 ||
        (!firstRender_ && (viewportWidth_ == newSize.x && viewportHeight_ == newSize.y))) {
        return;
    }

    viewportWidth_ = static_cast<unsigned int>(newSize.x);
    viewportHeight_ = static_cast<unsigned int>(newSize.y);

    int viewportSize[] = {static_cast<int>(newSize.x), static_cast<int>(newSize.y)};

    // Free old buffers
    // SPDLOG_DEBUG("First render? {}", firstRender_);
    // SPDLOG_DEBUG("Tex: {}, addr {}", tex_, (void *)&tex_);
    SPDLOG_DEBUG("Setting window size to ({}, {})", newSize.x, newSize.y);

    glDeleteTextures(1, &tex_);

    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportWidth_, viewportHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    renderWindow->InitializeFromCurrentContext();
    renderWindow->SetSize(viewportSize);
    interactor->SetSize(viewportSize);

    auto vtkfbo = renderWindow->GetDisplayFramebuffer();
    vtkfbo->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0);
    vtkfbo->UnBind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    firstRender_ = false;
}

void VtkViewer::setRenderer(const vtkSmartPointer<vtkRenderer> &renderer) {
    this->renderWindow->RemoveRenderer(this->renderer);
    this->renderer = renderer;
    this->renderWindow->AddRenderer(renderer);
}

// Added this myself to set the things that would have been set in the generic init.
void VtkViewer::setRenderWindow(const vtkSmartPointer<vtkGenericOpenGLRenderWindow> &renderWindow) {
    this->renderWindow = renderWindow;
    vtkSmartPointer<vtkCallbackCommand> isCurrentCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    isCurrentCallback->SetCallback(&isCurrentCallbackFn);
    renderWindow->AddObserver(vtkCommand::WindowIsCurrentEvent, isCurrentCallback);

    renderWindow->SwapBuffersOn();
    renderWindow->SetOffScreenRendering(true);
    renderWindow->SetFrameBlitModeToNoBlit();
}

void VtkViewer::start() { interactor->Start(); }
