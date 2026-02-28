cmake_minimum_required(VERSION 4.1)
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

find_package(OpenCV REQUIRED)

# For now laziliy just find all source files. It might be more than one exec later but eh
file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add source files
add_executable(theme-generator
    ${SRC_LIST}
)

target_compile_definitions(theme-generator PRIVATE
    SPDLOG_ACTIVE_LEVEL=$<IF:$<CONFIG:Debug>,SPDLOG_LEVEL_DEBUG,SPDLOG_LEVEL_INFO>
)

# Include directories, the opencv ones may be a bit odd due to the fetch.
target_include_directories(theme-generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(theme-generator PRIVATE ${OpenCV_LIBS} spdlog::spdlog)