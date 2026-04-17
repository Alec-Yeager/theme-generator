#pragma once

#include <glad/glad.h>

#include <exception>
#include <iostream>
#include <string>

#include "imgui.h"

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkProp.h>
#include <vtkPropCollection.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// RGB Color in range [0.0, 1.0]
#define DEFAULT_BACKGROUND 0.39, 0.39, 0.39
// Alpha value in range [0.0, 1.0] where 1 = opaque
#define DEFAULT_ALPHA 1

class VtkViewerError : public std::runtime_error {
public:
    explicit VtkViewerError(const std::string &message) throw() : std::runtime_error(message) {}
    ~VtkViewerError() = default;

public:
    virtual char const *what() const throw() { return exception::what(); }
};

class VtkViewer {
private:
    static void isCurrentCallbackFn(vtkObject *caller, long unsigned int eventId, void *clientData, void *callData);
    void processEvents();

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkGenericRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyleSwitch> interactorStyle;
    vtkSmartPointer<vtkRenderer> renderer;

private:
    unsigned int viewportWidth_, viewportHeight_;
    GLuint tex_;
    bool firstRender_;

public:
    VtkViewer();
    VtkViewer(const VtkViewer &vtkViewer);
    VtkViewer(VtkViewer &&vtkViewer) noexcept;
    ~VtkViewer();

    IMGUI_IMPL_API void init();
    VtkViewer &operator=(const VtkViewer &vtkViewer);

public:
    IMGUI_IMPL_API void render();
    IMGUI_IMPL_API void render(const ImVec2 size);
    IMGUI_IMPL_API void addActor(const vtkSmartPointer<vtkProp> &actor);
    IMGUI_IMPL_API void addActors(const vtkSmartPointer<vtkPropCollection> &actors);
    IMGUI_IMPL_API void removeActor(const vtkSmartPointer<vtkProp> &actor);
    void setViewportSize(const ImVec2 newSize);

    void setRenderer(const vtkSmartPointer<vtkRenderer> &renderer);

public:
    static inline unsigned int NoScrollFlags() {
        return ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    }

public:
    void setRenderWindow(const vtkSmartPointer<vtkGenericOpenGLRenderWindow> &renderWindow);

    inline void setInteractor(const vtkSmartPointer<vtkGenericRenderWindowInteractor> &interactor) {
        this->interactor = interactor;
    }

    inline void setInteractorStyle(const vtkSmartPointer<vtkInteractorStyleSwitch> &interactorStyle) {
        this->interactorStyle = interactorStyle;
    }

public:
    inline vtkSmartPointer<vtkGenericOpenGLRenderWindow> &getRenderWindow() { return renderWindow; }

    inline vtkSmartPointer<vtkGenericRenderWindowInteractor> &getInteractor() { return interactor; }

    inline vtkSmartPointer<vtkInteractorStyleSwitch> &getInteractorStyle() { return interactorStyle; }

    inline vtkSmartPointer<vtkRenderer> &getRenderer() { return renderer; }

public:
    inline unsigned int getViewportWidth() const { return viewportWidth_; }

    inline unsigned int getViewportHeight() const { return viewportHeight_; }

    inline unsigned int getTexture() const { return tex_; }
};
