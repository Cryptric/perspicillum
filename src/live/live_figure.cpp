#include "perspicillum/live_figure.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <stdexcept>
#include <mutex>

namespace perspicillum {

// ---------------------------------------------------------------------------
// Reference-counted GLFW initialisation
//
// Multiple LiveFigure instances (each with its own render thread and window)
// share a single glfwInit / glfwTerminate pair.  The mutex serialises the
// refcount update; actual GLFW calls happen only when the count transitions
// between 0 and 1.
//
// Note: glfwInit() is called from the render thread here.  This works on
// Linux (X11 / Wayland) but is not supported by GLFW on macOS, which requires
// all window operations on the main thread.
// ---------------------------------------------------------------------------

namespace {

std::mutex  g_glfw_mutex;
int         g_glfw_refcount = 0;

bool glfw_acquire() {
  std::lock_guard lock(g_glfw_mutex);
  if (g_glfw_refcount == 0) {
    if (!glfwInit()) return false;
  }
  ++g_glfw_refcount;
  return true;
}

void glfw_release() {
  std::lock_guard lock(g_glfw_mutex);
  if (--g_glfw_refcount == 0) {
    glfwTerminate();
  }
}

}  // namespace

// ---------------------------------------------------------------------------
// LiveSubplot::render
// ---------------------------------------------------------------------------

void LiveSubplot::render() {
  if (ImPlot::BeginPlot(
          title_.empty() ? "##live_plot" : title_.c_str(),
          ImVec2(-1, -1),
          ImPlotFlags_None)) {
    for (auto& plot : plots_) {
      plot->render();
    }
    ImPlot::EndPlot();
  }
}

// ---------------------------------------------------------------------------
// LiveFigure render loop
// ---------------------------------------------------------------------------

void LiveFigure::show() {
  if (running_.load())
    throw std::logic_error("LiveFigure::show() called while already running");

  running_.store(true);

  render_thread_ = std::thread([this]() {
    render_loop();
  });
}

void LiveFigure::render_loop() {
  if (!glfw_acquire()) {
    running_.store(false);
    return;
  }

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow* window = glfwCreateWindow(
      width_, height_,
      title_.empty() ? "Live Figure" : title_.c_str(),
      nullptr, nullptr);

  if (!window) {
    glfw_release();
    running_.store(false);
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // vsync

  // Each LiveFigure gets its own ImGui / ImPlot context so that multiple
  // live figures (or a mix of LiveFigure and the blocking Figure::show())
  // don't interfere with each other's contexts.
  IMGUI_CHECKVERSION();
  ImGuiContext*  imgui_ctx  = ImGui::CreateContext();
  ImPlotContext* implot_ctx = ImPlot::CreateContext();

  // Make these contexts current on this thread.
  ImGui::SetCurrentContext(imgui_ctx);
  ImPlot::SetCurrentContext(implot_ctx);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  ImPlot::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // --- event loop ---
  while (!glfwWindowShouldClose(window) && running_.load()) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Fill the entire OS window, no title bar, no padding.
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowBgAlpha(1.0f);
    constexpr ImGuiWindowFlags kFullscreen =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("##live_figure", nullptr, kFullscreen);
    render_frame();
    ImGui::End();

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // --- cleanup ---
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext(implot_ctx);
  ImGui::DestroyContext(imgui_ctx);
  glfwDestroyWindow(window);
  glfw_release();

  running_.store(false);
}

void LiveFigure::render_frame() {
  if (rows_ == 1 && cols_ == 1) {
    subplots_[0].render();
    return;
  }

  // Grid layout — mirrors Figure::render().
  ImVec2 available   = ImGui::GetContentRegionAvail();
  float  subplot_w   = available.x / static_cast<float>(cols_);
  float  subplot_h   = available.y / static_cast<float>(rows_);

  for (int row = 0; row < rows_; ++row) {
    for (int col = 0; col < cols_; ++col) {
      if (col > 0) ImGui::SameLine();

      ImGui::BeginChild(
          ImGui::GetID(reinterpret_cast<void*>(
              static_cast<uintptr_t>(row * cols_ + col))),
          ImVec2(subplot_w, subplot_h),
          false);

      int idx = row * cols_ + col;
      if (idx < static_cast<int>(subplots_.size())) {
        subplots_[idx].render();
      }

      ImGui::EndChild();
    }
  }
}

}  // namespace perspicillum
