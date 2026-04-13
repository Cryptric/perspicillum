#pragma once

// Live plotting support for perspicillum.
//
// LiveFigure opens a window in a background render thread so that show() does
// not block the caller.  All GLFW / ImGui / ImPlot / OpenGL calls are
// confined to that thread — this is safe on Linux (X11/Wayland).  On macOS,
// GLFW requires window operations on the main thread, which conflicts with
// the non-blocking design; macOS is therefore not supported by this module.
//
// Thread-safety contract
// ----------------------
//   • Add plots (add_scatter, add_line, add_bar) BEFORE calling show().
//     The plot list is not protected after the render thread starts.
//   • After show(), call update() on individual live plots from any thread.
//   • close() / wait() / the destructor are safe to call from any thread.

#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "live_plots.hpp"

namespace perspicillum {

// ---------------------------------------------------------------------------
// LiveSubplot
// ---------------------------------------------------------------------------

/// A subplot cell inside a LiveFigure.  Mirrors the Subplot API.
class LiveSubplot {
public:
  LiveSubplot(int rows, int cols, int index, const std::string& title = "")
      : title_(title), rows_(rows), cols_(cols), index_(index) {}

  LiveSubplot(const LiveSubplot&) = delete;
  LiveSubplot& operator=(const LiveSubplot&) = delete;
  LiveSubplot(LiveSubplot&&) = default;
  LiveSubplot& operator=(LiveSubplot&&) = default;

  /// Add a scatter plot with optional initial data.
  LiveScatter& add_scatter(const std::vector<double>& x = {},
                           const std::vector<double>& y = {}) {
    auto p = std::make_shared<LiveScatter>(x, y);
    auto* ptr = p.get();
    plots_.push_back(std::move(p));
    return *ptr;
  }

  /// Add a line plot with optional initial data.
  LiveLine& add_line(const std::vector<double>& x = {},
                     const std::vector<double>& y = {}) {
    auto p = std::make_shared<LiveLine>(x, y);
    auto* ptr = p.get();
    plots_.push_back(std::move(p));
    return *ptr;
  }

  /// Add a bar plot with optional initial data.
  LiveBar& add_bar(const std::vector<double>& x = {},
                   const std::vector<double>& y = {}) {
    auto p = std::make_shared<LiveBar>(x, y);
    auto* ptr = p.get();
    plots_.push_back(std::move(p));
    return *ptr;
  }

  std::string& title() { return title_; }
  const std::string& title() const { return title_; }

  /// Render all plots in this subplot (called from the render thread).
  void render();

private:
  std::vector<std::shared_ptr<LivePlotBase>> plots_;
  std::string title_;
  int rows_, cols_, index_;
};

// ---------------------------------------------------------------------------
// LiveFigure
// ---------------------------------------------------------------------------

/// A figure that renders in a background thread so that show() is non-blocking.
///
/// Typical usage:
/// @code
///   LiveFigure fig;
///   auto& s = fig.add_scatter(x0, y0);
///   fig.show();                         // returns immediately
///
///   while (running) {
///     // compute next frame …
///     s.update(new_x, new_y);           // safe from any thread
///   }
///
///   fig.close();
///   fig.wait();
/// @endcode
class LiveFigure {
public:
  LiveFigure() {
    subplots_.emplace_back(1, 1, 0);
  }

  LiveFigure(int rows, int cols) : rows_(rows), cols_(cols) {
    for (int i = 0; i < rows * cols; ++i) {
      subplots_.emplace_back(rows, cols, i);
    }
  }

  ~LiveFigure() {
    close();
    wait();
  }

  LiveFigure(const LiveFigure&) = delete;
  LiveFigure& operator=(const LiveFigure&) = delete;

  // --- subplot access (mirrors Figure::subplot) ---

  LiveSubplot& subplot(int row, int col = 1) {
    int idx = (row - 1) * cols_ + (col - 1);
    if (idx < 0 || idx >= static_cast<int>(subplots_.size()))
      throw std::out_of_range("Subplot index out of range");
    return subplots_[idx];
  }

  // --- convenience add methods on the default subplot ---

  /// Add a scatter plot to the default (first) subplot.
  LiveScatter& add_scatter(const std::vector<double>& x = {},
                           const std::vector<double>& y = {}) {
    ensure_not_running("add_scatter");
    return subplots_[0].add_scatter(x, y);
  }

  /// Add a line plot to the default (first) subplot.
  LiveLine& add_line(const std::vector<double>& x = {},
                     const std::vector<double>& y = {}) {
    ensure_not_running("add_line");
    return subplots_[0].add_line(x, y);
  }

  /// Add a bar plot to the default (first) subplot.
  LiveBar& add_bar(const std::vector<double>& x = {},
                   const std::vector<double>& y = {}) {
    ensure_not_running("add_bar");
    return subplots_[0].add_bar(x, y);
  }

  // --- configuration (call before show) ---

  LiveFigure& set_title(const std::string& title) {
    title_ = title;
    return *this;
  }

  LiveFigure& set_size(int width, int height) {
    width_ = width;
    height_ = height;
    return *this;
  }

  // --- lifecycle ---

  /// Open the window in a background render thread.  Returns immediately.
  /// Must not be called more than once per instance.
  void show();

  /// Signal the render thread to stop.  Safe to call from any thread.
  void close() { running_.store(false); }

  /// Block until the render thread has exited.
  void wait() {
    if (render_thread_.joinable())
      render_thread_.join();
  }

  /// Returns true while the render thread is alive.
  bool is_open() const { return running_.load(); }

private:
  void render_loop();
  void render_frame();

  void ensure_not_running(const char* method) const {
    if (running_.load())
      throw std::logic_error(
          std::string(method) +
          "() called after show() — add all plots before calling show()");
  }

  std::vector<LiveSubplot> subplots_;
  int rows_{1}, cols_{1};
  int width_{1200}, height_{800};
  std::string title_;

  std::thread render_thread_;
  std::atomic<bool> running_{false};
};

}  // namespace perspicillum
