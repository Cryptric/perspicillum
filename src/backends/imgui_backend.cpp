#include "perspicillum/window_manager.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <stdexcept>

namespace perspicillum {

WindowManager& WindowManager::instance() {
  static WindowManager instance;
  return instance;
}

bool WindowManager::initialize(int width, int height, const std::string& title) {
  if (initialized_) {
    return true;
  }

  // Initialize GLFW
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window_) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Enable vsync

  // Setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  ImPlot::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  initialized_ = true;
  return true;
}

bool WindowManager::is_open() const {
  return window_ && !glfwWindowShouldClose(window_);
}

bool WindowManager::begin_frame() {
  if (!initialized_) {
    return false;
  }

  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  return true;
}

void WindowManager::end_frame() {
  if (!initialized_) {
    return;
  }

  ImGui::Render();

  int display_w, display_h;
  glfwGetFramebufferSize(window_, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window_);
}

void WindowManager::set_render_callback(std::function<void()> callback) {
  render_callback_ = callback;
}

void WindowManager::show() {
  if (!initialized_) {
    throw std::runtime_error("WindowManager not initialized");
  }

  while (is_open()) {
    if (!begin_frame()) {
      break;
    }

    if (render_callback_) {
      render_callback_();
    }

    end_frame();
  }
}

void WindowManager::shutdown() {
  if (initialized_) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (window_) {
      glfwDestroyWindow(window_);
      window_ = nullptr;
    }

    glfwTerminate();
    initialized_ = false;
  }
}

}  // namespace perspicillum
