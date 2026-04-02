#include "perspicillum/plots.hpp"
#include <implot.h>
#include <imgui.h>

namespace perspicillum {

void LinePlot::render() {
  if (x_ && y_ && x_->size() == y_->size() && x_->size() > 0) {
    ImPlotSpec spec;
    spec.LineColor = ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    spec.LineWeight = style_.width;
    ImPlot::PlotLine(
        style_.name.empty() ? "Line" : style_.name.c_str(),
        x_->data(),
        y_->data(),
        static_cast<int>(x_->size()),
        spec
    );
  }
}

void ScatterPlot::render() {
  if (x_ && y_ && x_->size() == y_->size() && x_->size() > 0) {
    ImPlotSpec spec;
    spec.Marker = ImPlotMarker_Circle;
    spec.MarkerSize = static_cast<float>(style_.markersize);
    spec.MarkerFillColor = ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    ImPlot::PlotScatter(
        style_.name.empty() ? "Scatter" : style_.name.c_str(),
        x_->data(),
        y_->data(),
        static_cast<int>(x_->size()),
        spec
    );
  }
}

void BarPlot::render() {
  if (x_ && y_ && x_->size() == y_->size() && x_->size() > 0) {
    ImPlotSpec spec;
    spec.FillColor = ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    ImPlot::PlotBars(
        style_.name.empty() ? "Bar" : style_.name.c_str(),
        x_->data(),
        y_->data(),
        static_cast<int>(x_->size()),
        0.8,
        spec
    );
  }
}

}  // namespace perspicillum
