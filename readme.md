### Requirements

spdlog is currently managed by FetchContent.

- [opencv 4](https://opencv.org/)
- [spdlog](https://github.com/gabime/spdlog) <- included in the cmakelists
- [json.hpp](https://github.com/nlohmann/json) <- included in the cmakelists

For visualization (currently not optional, it will be)
- [vtk 9.6+](https://docs.vtk.org/en/latest/build_instructions/index.html)
- [glfw](https://www.glfw.org/)
- [glad](https://glad.dav1d.de/)

on Arch, this required: vtk gl2ps eigen3 utf8cpp freetype2 fast_float nlohmann_json and jsoncpp-cmake (AUR) from the optionals.

HSLuv implementation courtesy of https://github.com/hsluv/hsluv-c
Vtk-imgui compatibility implementation courtesy of https://github.com/trlsmax/imgui-vtk