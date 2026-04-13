#include "perspicillum/live_plots.hpp"

#include <imgui.h>
#include <implot.h>

namespace perspicillum {

// ---------------------------------------------------------------------------
// LiveScatter
// ---------------------------------------------------------------------------

LiveScatter::LiveScatter(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveScatter::LiveScatter(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveScatter::update(const std::vector<double>& x, const std::vector<double>& y) {
  std::lock_guard lock(mutex_);
  x_ = x;
  y_ = y;
}

void LiveScatter::update(std::span<const double> x, std::span<const double> y) {
  std::lock_guard lock(mutex_);
  x_.assign(x.begin(), x.end());
  y_.assign(y.begin(), y.end());
}

void LiveScatter::render() {
  std::lock_guard lock(mutex_);
  if (x_.empty() || x_.size() != y_.size()) return;

  ImPlotSpec spec;
  spec.Marker = ImPlotMarker_Circle;
  spec.MarkerSize = static_cast<float>(style_.markersize);
  spec.MarkerFillColor =
      ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);

  ImPlot::PlotScatter(
      style_.name.empty() ? "Scatter" : style_.name.c_str(),
      x_.data(), y_.data(), static_cast<int>(x_.size()), spec);
}

// ---------------------------------------------------------------------------
// LiveLine
// ---------------------------------------------------------------------------

LiveLine::LiveLine(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveLine::LiveLine(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveLine::update(const std::vector<double>& x, const std::vector<double>& y) {
  std::lock_guard lock(mutex_);
  x_ = x;
  y_ = y;
}

void LiveLine::update(std::span<const double> x, std::span<const double> y) {
  std::lock_guard lock(mutex_);
  x_.assign(x.begin(), x.end());
  y_.assign(y.begin(), y.end());
}

void LiveLine::render() {
  std::lock_guard lock(mutex_);
  if (x_.empty() || x_.size() != y_.size()) return;

  ImPlotSpec spec;
  spec.LineColor =
      ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
  spec.LineWeight = style_.width;

  ImPlot::PlotLine(
      style_.name.empty() ? "Line" : style_.name.c_str(),
      x_.data(), y_.data(), static_cast<int>(x_.size()), spec);
}

// ---------------------------------------------------------------------------
// LiveBar
// ---------------------------------------------------------------------------

LiveBar::LiveBar(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveBar::LiveBar(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveBar::update(const std::vector<double>& x, const std::vector<double>& y) {
  std::lock_guard lock(mutex_);
  x_ = x;
  y_ = y;
}

void LiveBar::update(std::span<const double> x, std::span<const double> y) {
  std::lock_guard lock(mutex_);
  x_.assign(x.begin(), x.end());
  y_.assign(y.begin(), y.end());
}

void LiveBar::render() {
  std::lock_guard lock(mutex_);
  if (x_.empty() || x_.size() != y_.size()) return;

  ImPlotSpec spec;
  spec.FillColor =
      ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);

  ImPlot::PlotBars(
      style_.name.empty() ? "Bar" : style_.name.c_str(),
      x_.data(), y_.data(), static_cast<int>(x_.size()), 0.8, spec);
}

}  // namespace perspicillum
