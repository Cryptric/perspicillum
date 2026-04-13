#pragma once

#include <mutex>
#include <span>
#include <vector>
#include "plot_style.hpp"

#ifdef PERSPICILLUM_WITH_EIGEN
#include <Eigen/Core>
#endif

namespace perspicillum {

/// Base class for live (updatable) plots.
/// render() is called exclusively from the LiveFigure render thread.
/// update() may be called safely from any thread.
class LivePlotBase {
public:
  virtual ~LivePlotBase() = default;
  virtual void render() = 0;
  virtual PlotStyle& style() = 0;
  virtual const PlotStyle& style() const = 0;
};

// ---------------------------------------------------------------------------
// LiveScatter
// ---------------------------------------------------------------------------

/// Thread-safe scatter plot whose data can be updated while the window is open.
class LiveScatter : public LivePlotBase {
public:
  LiveScatter() = default;
  LiveScatter(const std::vector<double>& x, const std::vector<double>& y);
  LiveScatter(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  LiveScatter(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Replace all plotted data. Safe to call from any thread.
  void update(const std::vector<double>& x, const std::vector<double>& y);
  void update(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  void update(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Append a single point. Safe to call from any thread.
  void append(double x, double y) {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }

private:
  mutable std::mutex mutex_;
  std::vector<double> x_, y_;
  PlotStyle style_;

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename D>
  static void eigen_assign(const Eigen::MatrixBase<D>& src, std::vector<double>& dst) {
    if (src.cols() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.rows());
    } else if (src.rows() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.cols());
    } else {
      throw std::invalid_argument("Expected a 1D Eigen vector");
    }
  }
#endif
};

// ---------------------------------------------------------------------------
// LiveLine
// ---------------------------------------------------------------------------

/// Thread-safe line plot whose data can be updated while the window is open.
class LiveLine : public LivePlotBase {
public:
  LiveLine() = default;
  LiveLine(const std::vector<double>& x, const std::vector<double>& y);
  LiveLine(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  LiveLine(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Replace all plotted data. Safe to call from any thread.
  void update(const std::vector<double>& x, const std::vector<double>& y);
  void update(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  void update(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Append a single point. Safe to call from any thread.
  void append(double x, double y) {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }

private:
  mutable std::mutex mutex_;
  std::vector<double> x_, y_;
  PlotStyle style_;

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename D>
  static void eigen_assign(const Eigen::MatrixBase<D>& src, std::vector<double>& dst) {
    if (src.cols() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.rows());
    } else if (src.rows() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.cols());
    } else {
      throw std::invalid_argument("Expected a 1D Eigen vector");
    }
  }
#endif
};

// ---------------------------------------------------------------------------
// LiveBar
// ---------------------------------------------------------------------------

/// Thread-safe bar plot whose data can be updated while the window is open.
class LiveBar : public LivePlotBase {
public:
  LiveBar() = default;
  LiveBar(const std::vector<double>& x, const std::vector<double>& y);
  LiveBar(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  LiveBar(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Replace all plotted data. Safe to call from any thread.
  void update(const std::vector<double>& x, const std::vector<double>& y);
  void update(std::span<const double> x, std::span<const double> y);

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename DX, typename DY>
  void update(const Eigen::MatrixBase<DX>& x, const Eigen::MatrixBase<DY>& y) {
    std::lock_guard lock(mutex_);
    eigen_assign(x, x_);
    eigen_assign(y, y_);
  }
#endif

  /// Append a single point. Safe to call from any thread.
  void append(double x, double y) {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }

  void render() override;
  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }

private:
  mutable std::mutex mutex_;
  std::vector<double> x_, y_;
  PlotStyle style_;

#ifdef PERSPICILLUM_WITH_EIGEN
  template <typename D>
  static void eigen_assign(const Eigen::MatrixBase<D>& src, std::vector<double>& dst) {
    if (src.cols() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.rows());
    } else if (src.rows() == 1) {
      dst.assign(src.derived().data(), src.derived().data() + src.cols());
    } else {
      throw std::invalid_argument("Expected a 1D Eigen vector");
    }
  }
#endif
};

}  // namespace perspicillum
