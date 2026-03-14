### Requirements

spdlog is currently managed by FetchContent.

- [opencv 4](https://opencv.org/)
- [spdlog](https://github.com/gabime/spdlog)

For visualization (currently not optional, it will be)
- [vtk 9.1+](https://docs.vtk.org/en/latest/build_instructions/index.html)

on Arch, this required: vtk gl2ps eigen3 utf8cpp freetype2 fast_float nlohmann_json and jsoncpp-cmake (AUR) from the optionals.

HSLuv implementation courtesy of https://github.com/hsluv/hsluv-c