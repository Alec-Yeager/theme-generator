#include "BasicVTK.hpp"

BasicVTK::BasicVTK() {}

BasicVTK::~BasicVTK() {}

void BasicVTK::run() {

    sphere_source_->SetCenter(0, 0, 0);
    sphere_source_->SetRadius(5);
    sphere_source_->Update();

    prop_->SetColor(0, 1, 1);
    prop_->SetEdgeVisibility(true);
    prop_->SetEdgeColor(1, 0, 1);

    iren_->SetInteractorStyle(camera_interactor_style_);
    // Keeping this so I remember how the addObserver call worked.
    // iren_->AddObserver(vtkCommand::ExitEvent, this, &BasicVTK::exit);

    // Then set up all the connections
    mapper_->SetInputConnection(sphere_source_->GetOutputPort());
    actor_->SetMapper(mapper_);
    actor_->SetProperty(prop_);
    ren_->AddActor(actor_);
    win_->AddRenderer(ren_);
    iren_->SetRenderWindow(win_);

    iren_->Render();
    iren_->Start();

    return;
}

// relying on internal exit didn't work for some reason.
void BasicVTK::exit() { // I seriously thought I needed this. At least now I know how callbacks work.
}
