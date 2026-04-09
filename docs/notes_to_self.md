### Using the vtk and imgui glue

Seems like this is all reliant on an opengl context being set in the main initialization of the glfw window.\
Not seeing an obvious place where the glfw window is set to be the render target or anything.\
Although this seems to be important from the resize function: \
"Initialize the render window from the information associated
with the currently activated OpenGL context."\
which is a tooltip from renderWindow->InitializeFromCurrentContext();
