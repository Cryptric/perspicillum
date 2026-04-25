#pragma once

#include <memory>
#include "figure.hpp"
#include "data_container.hpp"

namespace perspicillum {

/// Express-style quick plot builder
class PlotExpression {
private:
  std::unique_ptr<Figure> figure_;
  std::unique_ptr<PlotBase> plot_;

public:
  PlotExpression(std::unique_ptr<Figure> fig, std::unique_ptr<PlotBase> plt)
      : figure_(std::move(fig)), plot_(std::move(plt)) {}

  /// Set plot name (label)
  PlotExpression& name(const std::string& n) {
    plot_->style().set_name(n);
    return *this;
  }

  /// Set plot color (RGBA normalized 0-1)
  PlotExpression& color(float r, float g, float b, float a = 1.0f) {
    plot_->style().set_color(r, g, b, a);
    return *this;
  }

  /// Set marker size
  PlotExpression& markersize(int size) {
    plot_->style().set_markersize(size);
    return *this;
  }

  /// Set line width
  PlotExpression& width(float w) {
    plot_->style().set_width(w);
    return *this;
  }

  /// Set title
  PlotExpression& title(const std::string& t) {
    figure_->set_title(t);
    return *this;
  }

  /// Show the plot window
  void show() {
    figure_->show();
  }

  Figure* get_figure() { return figure_.get(); }
};

// ============================================================================
// Express API Functions
// ============================================================================

/// Quick plot with line
inline PlotExpression plot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
  auto fig = std::make_unique<Figure>();
  auto line = std::make_unique<LinePlot>(std::move(x), std::move(y));
  auto* ptr = line.get();
  fig->add_plot(std::move(line));
  return PlotExpression(std::move(fig), std::unique_ptr<PlotBase>(ptr));
}

/// Quick scatter plot
inline PlotExpression scatter(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
  auto fig = std::make_unique<Figure>();
  auto scatter = std::make_unique<ScatterPlot>(std::move(x), std::move(y));
  auto* ptr = scatter.get();
  fig->add_plot(std::move(scatter));
  return PlotExpression(std::move(fig), std::unique_ptr<PlotBase>(ptr));
}

/// Quick bar plot
inline PlotExpression bar(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y) {
  auto fig = std::make_unique<Figure>();
  auto bar = std::make_unique<BarPlot>(std::move(x), std::move(y));
  auto* ptr = bar.get();
  fig->add_plot(std::move(bar));
  return PlotExpression(std::move(fig), std::unique_ptr<PlotBase>(ptr));
}

// Convenience overloads for std::vector<double>
inline PlotExpression plot(const std::vector<double>& x, const std::vector<double>& y) {
  return plot(make_data(x), make_data(y));
}

inline PlotExpression scatter(const std::vector<double>& x, const std::vector<double>& y) {
  return scatter(make_data(x), make_data(y));
}

inline PlotExpression bar(const std::vector<double>& x, const std::vector<double>& y) {
  return bar(make_data(x), make_data(y));
}

#ifdef PERSPICILLUM_WITH_EIGEN
// Convenience overloads for Eigen vectors
template <typename DerivedX, typename DerivedY>
inline PlotExpression plot(const Eigen::MatrixBase<DerivedX>& x, 
                           const Eigen::MatrixBase<DerivedY>& y) {
  return plot(make_data(x), make_data(y));
}

template <typename DerivedX, typename DerivedY>
inline PlotExpression scatter(const Eigen::MatrixBase<DerivedX>& x, 
                              const Eigen::MatrixBase<DerivedY>& y) {
  return scatter(make_data(x), make_data(y));
}

template <typename DerivedX, typename DerivedY>
inline PlotExpression bar(const Eigen::MatrixBase<DerivedX>& x, 
                          const Eigen::MatrixBase<DerivedY>& y) {
  return bar(make_data(x), make_data(y));
}
#endif

// ============================================================================
// HeatmapExpression
// ============================================================================

/// Express-style quick heatmap builder
class HeatmapExpression {
private:
  std::unique_ptr<Figure> figure_;
  HeatmapPlot* plot_;

public:
  HeatmapExpression(std::unique_ptr<Figure> fig, HeatmapPlot* plot)
      : figure_(std::move(fig)), plot_(plot) {}

  HeatmapExpression& name(const std::string& n) {
    plot_->style().set_name(n);
    return *this;
  }

  HeatmapExpression& title(const std::string& t) {
    figure_->set_title(t);
    return *this;
  }

  HeatmapExpression& colormap(Colormap cm) {
    plot_->set_colormap(cm);
    return *this;
  }

  HeatmapExpression& scale(double min, double max) {
    plot_->set_scale(min, max);
    return *this;
  }

  HeatmapExpression& show_labels(bool show = true) {
    plot_->set_show_labels(show);
    return *this;
  }

  void show() {
    figure_->show();
  }

  Figure* get_figure() { return figure_.get(); }
};

// ============================================================================
// heatmap() express functions
// ============================================================================

inline HeatmapExpression heatmap(std::vector<double> values, int rows, int cols) {
  auto fig = std::make_unique<Figure>();
  auto hm = std::make_unique<HeatmapPlot>(std::move(values), rows, cols);
  auto* ptr = hm.get();
  fig->add_plot(std::move(hm));
  return HeatmapExpression(std::move(fig), ptr);
}

inline HeatmapExpression heatmap(std::span<const double> values, int rows, int cols) {
  auto fig = std::make_unique<Figure>();
  auto hm = std::make_unique<HeatmapPlot>(values, rows, cols);
  auto* ptr = hm.get();
  fig->add_plot(std::move(hm));
  return HeatmapExpression(std::move(fig), ptr);
}

#ifdef PERSPICILLUM_WITH_EIGEN
template <typename Derived>
inline HeatmapExpression heatmap(const Eigen::MatrixBase<Derived>& mat) {
  auto fig = std::make_unique<Figure>();
  auto hm = std::make_unique<HeatmapPlot>(mat);
  auto* ptr = hm.get();
  fig->add_plot(std::move(hm));
  return HeatmapExpression(std::move(fig), ptr);
}
#endif

}  // namespace perspicillum
