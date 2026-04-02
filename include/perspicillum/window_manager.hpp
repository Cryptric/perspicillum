#pragma once

#include <string>
#include <functional>

struct GLFWwindow;

namespace perspicillum {

/// Manages window creation and ImGui/ImPlot setup
class WindowManager {
public:
  static WindowManager& instance();
  
  WindowManager(const WindowManager&) = delete;
  WindowManager& operator=(const WindowManager&) = delete;

  /// Initialize window manager (call once at startup)
  bool initialize(int width = 1200, int height = 800, const std::string& title = "Plot");
  
  /// Check if window is still open
  bool is_open() const;
  
  /// Process window events and begin frame
  bool begin_frame();
  
  /// End frame and render
  void end_frame();
  
  /// Set a callback to render custom content
  void set_render_callback(std::function<void()> callback);
  
  /// Show the window (blocking)
  void show();
  
  /// Shutdown window manager
  void shutdown();
  
  GLFWwindow* get_window() { return window_; }

private:
  WindowManager() = default;
  
  GLFWwindow* window_{nullptr};
  std::function<void()> render_callback_;
  bool initialized_{false};
};

}  // namespace perspicillum
