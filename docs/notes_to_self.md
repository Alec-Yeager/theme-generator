### Using the vtk and imgui glue

Seems like this is all reliant on an opengl context being set in the main initialization of the glfw window.\
Not seeing an obvious place where the glfw window is set to be the render target or anything.\
Although this seems to be important from the resize function: \
"Initialize the render window from the information associated
with the currently activated OpenGL context."\
which is a tooltip from renderWindow->InitializeFromCurrentContext();

### glfw and gald and imgui
Need to refactor so that all those init calls are only made ONCE. Don't have the visualizer be recreated in a loop, instead have it be the main window manager and give it setter methods for replays. All of the glfw, imgui, etc etc are context dependent, and have singular setup and terminates that we'd like to call. All of that should be in a main window that is managed by a class.
