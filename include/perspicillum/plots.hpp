#pragma once

#include <span>
#include <vector>
#include "plot_base.hpp"

namespace perspicillum {

/// Line plot
class LinePlot : public PlotBase {
private:
  std::unique_ptr<DataContainer> x_;
  std::unique_ptr<DataContainer> y_;
  PlotStyle style_;

public:
  LinePlot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y)
      : x_(std::move(x)), y_(std::move(y)) {}
  
  // Convenience constructors for common data types
  LinePlot(const std::vector<double>& x, const std::vector<double>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
  
  LinePlot(std::span<const double> x, std::span<const double> y)
      : x_(make_data(x)), y_(make_data(y)) {}

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DerivedX, typename DerivedY>
  LinePlot(const Eigen::MatrixBase<DerivedX>& x, const Eigen::MatrixBase<DerivedY>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
#endif

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }
};

/// Scatter plot
class ScatterPlot : public PlotBase {
private:
  std::unique_ptr<DataContainer> x_;
  std::unique_ptr<DataContainer> y_;
  PlotStyle style_;

public:
  ScatterPlot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y)
      : x_(std::move(x)), y_(std::move(y)) {}
  
  // Convenience constructors for common data types
  ScatterPlot(const std::vector<double>& x, const std::vector<double>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
  
  ScatterPlot(std::span<const double> x, std::span<const double> y)
      : x_(make_data(x)), y_(make_data(y)) {}

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DerivedX, typename DerivedY>
  ScatterPlot(const Eigen::MatrixBase<DerivedX>& x, const Eigen::MatrixBase<DerivedY>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
#endif

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }
};

/// Bar plot
class BarPlot : public PlotBase {
private:
  std::unique_ptr<DataContainer> x_;
  std::unique_ptr<DataContainer> y_;
  PlotStyle style_;

public:
  BarPlot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y)
      : x_(std::move(x)), y_(std::move(y)) {}
  
  // Convenience constructors for common data types
  BarPlot(const std::vector<double>& x, const std::vector<double>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
  
  BarPlot(std::span<const double> x, std::span<const double> y)
      : x_(make_data(x)), y_(make_data(y)) {}

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DerivedX, typename DerivedY>
  BarPlot(const Eigen::MatrixBase<DerivedX>& x, const Eigen::MatrixBase<DerivedY>& y)
      : x_(make_data(x)), y_(make_data(y)) {}
#endif

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }
};

/// Heatmap (2D color grid) plot
class HeatmapPlot : public PlotBase {
private:
  std::vector<double> values_;
  int rows_, cols_;
  PlotStyle style_;
  Colormap colormap_{Colormap::Viridis};
  double scale_min_{0}, scale_max_{0};  // both 0 = auto
  bool show_labels_{false};
  bool col_major_{false};

public:
  HeatmapPlot(std::vector<double> values, int rows, int cols)
      : values_(std::move(values)), rows_(rows), cols_(cols) {}

  HeatmapPlot(std::span<const double> values, int rows, int cols)
      : values_(values.begin(), values.end()), rows_(rows), cols_(cols) {}

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename Derived>
  explicit HeatmapPlot(const Eigen::MatrixBase<Derived>& mat)
      : values_(mat.derived().data(), mat.derived().data() + mat.size()),
        rows_(static_cast<int>(mat.rows())),
        cols_(static_cast<int>(mat.cols())),
        col_major_(true) {}
#endif

  HeatmapPlot& set_colormap(Colormap cm) { colormap_ = cm; return *this; }
  HeatmapPlot& set_scale(double min, double max) { scale_min_ = min; scale_max_ = max; return *this; }
  HeatmapPlot& set_show_labels(bool show) { show_labels_ = show; return *this; }

  // Accessors for the renderer
  Colormap colormap() const { return colormap_; }
  double scale_min() const { return scale_min_; }
  double scale_max() const { return scale_max_; }
  bool show_labels() const { return show_labels_; }
  bool col_major() const { return col_major_; }
  int rows() const { return rows_; }
  int cols() const { return cols_; }
  const double* raw_data() const { return values_.data(); }

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }
};

}  // namespace perspicillum
