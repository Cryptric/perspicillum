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
target_include_directories(imgui INTERFACE
  $<BUILD_INTERFACE:${EXTERNAL_DIR}/imgui>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(implot INTERFACE
  $<BUILD_INTERFACE:${EXTERNAL_DIR}/implot>
  $<BUILD_INTERFACE:${EXTERNAL_DIR}/imgui>
  $<INSTALL_INTERFACE:include>
)

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${EXTERNAL_DIR}/imgui>
    $<BUILD_INTERFACE:${EXTERNAL_DIR}/imgui/backends>
    $<BUILD_INTERFACE:${EXTERNAL_DIR}/implot>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/perspicillum")

# Install library and interface targets
install(TARGETS perspicillum imgui implot
  EXPORT perspicillumTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Export the targets to a script
install(EXPORT perspicillumTargets
  FILE perspicillumTargets.cmake
  NAMESPACE perspicillum::
  DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Create and install the Config file
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/perspicillumConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/perspicillumConfig.cmake"
  INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/perspicillumConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/perspicillumConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/perspicillumConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
)
