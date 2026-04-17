cmake_minimum_required(VERSION 3.5...3.31)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(theme-generator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Install spdlog my beloved
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

# Add in the imgui
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/backends")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/misc/cpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/include")

find_package(jsoncpp REQUIRED)
# find_package(nlohmann_json REQUIRED)
find_package(OpenCV REQUIRED)
# find_package(OpenGL REQUIRED)
find_package(VTK COMPONENTS 
    CommonColor
    CommonCore
    FiltersSources
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingOpenGL2
)

# For now laziliy just find all source files. It might be more than one exec later but eh
file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB CLUSTERING_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/*.cpp")
file(GLOB COLORING_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/coloring/*.cpp")
file(GLOB GEOMETRY_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/*.cpp")
file(GLOB VISUALIZATION_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/visualization/*.cpp")
file(GLOB DEP_IMGUI_LIST "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/*.cpp")
file(GLOB DEP_GLAD_LIST "${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/src/*.c")
set(DEP_IMGUI_BACKENDS "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/backends/imgui_impl_glfw.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/backends/imgui_impl_opengl3.cpp")


# Add source files
add_executable(theme-generator
    ${SRC_LIST}
    ${CLUSTERING_LIST}
    ${COLORING_LIST}
    ${GEOMETRY_LIST}
    ${VISUALIZATION_LIST}
    ${DEP_IMGUI_LIST}
    ${DEP_IMGUI_BACKENDS}
    ${DEP_GLAD_LIST}
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/misc/cpp/imgui_stdlib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/coloring/hsluv.c
)


target_compile_definitions(theme-generator PRIVATE
    SPDLOG_ACTIVE_LEVEL=$<IF:$<CONFIG:Debug>,SPDLOG_LEVEL_DEBUG,SPDLOG_LEVEL_INFO>
)

# Include directories, the opencv ones may be a bit odd due to the fetch.
target_include_directories(theme-generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(theme-generator PRIVATE ${OpenCV_LIBS} spdlog::spdlog ${VTK_LIBRARIES} VTK::glad glfw)

add_executable(minimal-imgui-vtk
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualization/demo/imgui-vtk-demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualization/VtkViewer.cpp
    ${DEP_IMGUI_LIST}
    ${DEP_IMGUI_BACKENDS}
    ${DEP_GLAD_LIST}
 )

 target_compile_definitions(minimal-imgui-vtk PRIVATE
    SPDLOG_ACTIVE_LEVEL=$<IF:$<CONFIG:Debug>,SPDLOG_LEVEL_DEBUG,SPDLOG_LEVEL_INFO>
)

target_include_directories(minimal-imgui-vtk PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(minimal-imgui-vtk PRIVATE ${OpenCV_LIBS} spdlog::spdlog ${VTK_LIBRARIES} VTK::glad glfw)