#include "perspicillum/figure.hpp"
#include "perspicillum/window_manager.hpp"
#include <implot.h>
#include <imgui.h>
#include <glm/glm.hpp>

namespace perspicillum {

void Subplot::render() {
  if (ImPlot::BeginPlot(
    title_.empty() ? "Plot" : title_.c_str(),
    ImVec2(-1, -1),
    ImPlotFlags_None
  )) {
    if (x_range_.set)
      ImPlot::SetupAxisLimits(ImAxis_X1, x_range_.lo, x_range_.hi,
                              x_range_.locked ? ImPlotCond_Always : ImPlotCond_Once);
    if (y_range_.set)
      ImPlot::SetupAxisLimits(ImAxis_Y1, y_range_.lo, y_range_.hi,
                              y_range_.locked ? ImPlotCond_Always : ImPlotCond_Once);
    for (auto& plot : plots_) {
      plot->render();
    }
    ImPlot::EndPlot();
  }
}

void Figure::render() {
  // Create a layout for subplots
  if (rows_ == 1 && cols_ == 1) {
    // Single subplot - use all available space
    ImVec2 available = ImGui::GetContentRegionAvail();
    ImGui::SetNextItemWidth(available.x);
    subplots_[0].render();
  } else {
    // Multiple subplots - arrange in grid with responsive sizing
    ImVec2 available = ImGui::GetContentRegionAvail();
    float subplot_width = available.x / cols_;
    float subplot_height = available.y / rows_;

    for (int row = 0; row < rows_; ++row) {
      for (int col = 0; col < cols_; ++col) {
        if (col > 0) {
          ImGui::SameLine();
        }

        ImGui::BeginChild(
            ImGui::GetID(reinterpret_cast<void*>(static_cast<uintptr_t>(row * cols_ + col))),
            ImVec2(subplot_width, subplot_height),
            false  // No border for cleaner look
        );

        int idx = row * cols_ + col;
        if (idx < static_cast<int>(subplots_.size())) {
          subplots_[idx].render();
        }

        ImGui::EndChild();
      }
    }
  }
}

void Figure::show() {
  auto& wm = WindowManager::instance();
  wm.initialize(width_, height_, title_.empty() ? "Figure" : title_);

  wm.set_render_callback([this]() {
    // Always fill entire window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Plot", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    this->render();
    ImGui::End();
  });

  wm.show();
  wm.shutdown();
}

}  // namespace perspicillum
