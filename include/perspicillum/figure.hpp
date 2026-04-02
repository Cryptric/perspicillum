#pragma once

#include <vector>
#include <memory>
#include <string>
#include "plot_base.hpp"
#include "plots.hpp"

namespace perspicillum {

/// Represents a subplot within a figure
class Subplot {
private:
  std::vector<std::unique_ptr<PlotBase>> plots_;
  std::string title_;
  int rows_, cols_, index_;

public:
  Subplot(int rows, int cols, int index, const std::string& title = "")
      : title_(title), rows_(rows), cols_(cols), index_(index) {}
  
  Subplot(const Subplot&) = delete;
  Subplot& operator=(const Subplot&) = delete;
  Subplot(Subplot&&) = default;
  Subplot& operator=(Subplot&&) = default;

  /// Add a plot to this subplot
  PlotBase& add_plot(std::unique_ptr<PlotBase> plot) {
    auto* ptr = plot.get();
    plots_.push_back(std::move(plot));
    return *ptr;
  }

  /// Template version for type safety
  template<typename PlotType>
  PlotType& add_plot(std::unique_ptr<PlotType> plot) {
    auto* ptr = plot.get();
    plots_.push_back(std::move(plot));
    return *ptr;
  }

  /// Direct plot object overload - accepts plot by rvalue ref
  template<typename PlotType>
  PlotType& add_plot(PlotType&& plot) {
    auto plot_ptr = std::make_unique<PlotType>(std::move(plot));
    auto* ptr = plot_ptr.get();
    plots_.push_back(std::move(plot_ptr));
    return *ptr;
  }

  /// Render all plots in this subplot
  void render();

  /// Get mutable reference to title
  std::string& title() { return title_; }
  const std::string& title() const { return title_; }
};

/// Main figure class (Plotly-like API)
class Figure {
private:
  std::vector<Subplot> subplots_;
  int rows_{1}, cols_{1};
  int width_{1200}, height_{800};
  std::string title_;

public:
  Figure() : subplots_() {
    subplots_.emplace_back(1, 1, 0);
  }

  Figure(int rows, int cols) : rows_(rows), cols_(cols), subplots_() {
    for (int i = 0; i < rows * cols; ++i) {
      subplots_.emplace_back(rows, cols, i);
    }
  }

  /// Get subplot at position (1-indexed like Plotly)
  Subplot& subplot(int row, int col = 1) {
    int idx = (row - 1) * cols_ + (col - 1);
    if (idx < 0 || idx >= static_cast<int>(subplots_.size())) {
      throw std::out_of_range("Subplot index out of range");
    }
    return subplots_[idx];
  }

  /// Add a line plot to the default subplot
  LinePlot& add_line(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
    auto plot = std::make_unique<LinePlot>(std::move(x), std::move(y));
    auto* ptr = plot.get();
    subplots_[0].add_plot(std::move(plot));
    return *ptr;
  }

  /// Add a scatter plot to the default subplot
  ScatterPlot& add_scatter(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
    auto plot = std::make_unique<ScatterPlot>(std::move(x), std::move(y));
    auto* ptr = plot.get();
    subplots_[0].add_plot(std::move(plot));
    return *ptr;
  }

  /// Add a bar plot to the default subplot
  BarPlot& add_bar(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
    auto plot = std::make_unique<BarPlot>(std::move(x), std::move(y));
    auto* ptr = plot.get();
    subplots_[0].add_plot(std::move(plot));
    return *ptr;
  }

  /// Set figure title
  Figure& set_title(const std::string& title) {
    title_ = title;
    return *this;
  }

  /// Set figure size
  Figure& set_size(int width, int height) {
    width_ = width;
    height_ = height;
    return *this;
  }

  /// Render the entire figure
  void render();

  /// Show the figure in a window (blocking)
  void show();

  /// Helper for express API to add plots
  template<typename PlotType>
  void add_plot(std::unique_ptr<PlotType> plot) {
    subplots_[0].add_plot(std::move(plot));
  }

  int get_width() const { return width_; }
  int get_height() const { return height_; }
};

}  // namespace perspicillum
