#pragma once

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

}  // namespace perspicillum
