cmake_minimum_required(VERSION 3.20)
project(perspicillum VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(PERSPICILLUM_BUILD_EXAMPLES "Build example programs" ON)
option(PERSPICILLUM_BUILD_TESTS "Build tests" ON)
option(PERSPICILLUM_WITH_EIGEN "Include Eigen support" ON)

# Find dependencies
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(PkgConfig QUIET)

if(PERSPICILLUM_WITH_EIGEN)
  find_package(Eigen3 3.3 REQUIRED NO_MODULE)
  add_compile_definitions(PERSPICILLUM_WITH_EIGEN)
endif()

# External dependencies as submodules/headers
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# Create interface library for imgui + implot
add_library(imgui INTERFACE)
add_library(implot INTERFACE)

# Set up include paths
target_include_directories(imgui INTERFACE "${EXTERNAL_DIR}/imgui")
target_include_directories(implot INTERFACE "${EXTERNAL_DIR}/implot" "${EXTERNAL_DIR}/imgui")

# ImGui implementation files (needed to be compiled with the main library)
set(IMGUI_SOURCES
  "${EXTERNAL_DIR}/imgui/imgui.cpp"
  "${EXTERNAL_DIR}/imgui/imgui_demo.cpp"
  "${EXTERNAL_DIR}/imgui/imgui_draw.cpp"
  "${EXTERNAL_DIR}/imgui/imgui_widgets.cpp"
  "${EXTERNAL_DIR}/imgui/imgui_tables.cpp"
  "${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp"
  "${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# ImPlot implementation files
set(IMPLOT_SOURCES
  "${EXTERNAL_DIR}/implot/implot.cpp"
  "${EXTERNAL_DIR}/implot/implot_items.cpp"
)

# Main library
add_library(perspicillum STATIC
  src/backends/imgui_backend.cpp
  src/backends/implot_adapter.cpp
  src/core/figure.cpp
  ${IMGUI_SOURCES}
  ${IMPLOT_SOURCES}
)

target_include_directories(perspicillum
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${EXTERNAL_DIR}/imgui"
    "${EXTERNAL_DIR}/imgui/backends"
    "${EXTERNAL_DIR}/implot"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(perspicillum
  PUBLIC
    glfw
    OpenGL::GL
    glm::glm
    imgui
    implot
)

if(PERSPICILLUM_WITH_EIGEN)
  target_link_libraries(perspicillum PUBLIC Eigen3::Eigen)
endif()

# Examples
if(PERSPICILLUM_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Tests
if(PERSPICILLUM_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
