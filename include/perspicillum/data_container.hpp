#pragma once

#include <vector>
#include <memory>
#include <span>
#include <string>

#ifdef PERSPICILLUM_WITH_EIGEN
#include <Eigen/Core>
#endif

namespace perspicillum {

/// Base class for data containers
class DataContainer {
public:
  virtual ~DataContainer() = default;
  virtual const double* data() const = 0;
  virtual size_t size() const = 0;
  virtual const char* name() const { return ""; }
};

/// Container for std::vector<double>
class VectorData : public DataContainer {
private:
  std::vector<double> data_;
  std::string name_;

public:
  explicit VectorData(std::vector<double> data, const std::string& name = "")
      : data_(std::move(data)), name_(name) {}

  explicit VectorData(std::span<const double> data, const std::string& name = "")
      : data_(data.begin(), data.end()), name_(name) {}

  const double* data() const override { return data_.data(); }
  size_t size() const override { return data_.size(); }
  const char* name() const override { return name_.c_str(); }
};

#ifdef PERSPICILLUM_WITH_EIGEN
/// Container for Eigen vectors
class EigenVectorData : public DataContainer {
private:
  std::vector<double> data_;
  std::string name_;

public:
  template <typename Derived>
  explicit EigenVectorData(const Eigen::MatrixBase<Derived>& vec, const std::string& name = "")
      : name_(name) {
    // Handle both column and row vectors
    if (vec.cols() == 1) {
      data_.assign(vec.derived().data(), vec.derived().data() + vec.rows());
    } else if (vec.rows() == 1) {
      data_.assign(vec.derived().data(), vec.derived().data() + vec.cols());
    } else {
      throw std::invalid_argument("EigenVectorData expects a vector (1D), not a matrix");
    }
  }

  const double* data() const override { return data_.data(); }
  size_t size() const override { return data_.size(); }
  const char* name() const override { return name_.c_str(); }
};
#endif

/// Helper to create VectorData from std::vector
inline std::unique_ptr<DataContainer> make_data(std::vector<double> data, 
                                                  const std::string& name = "") {
  return std::make_unique<VectorData>(std::move(data), name);
}

/// Helper to create VectorData from span
inline std::unique_ptr<DataContainer> make_data(std::span<const double> data,
                                                  const std::string& name = "") {
  return std::make_unique<VectorData>(data, name);
}

#ifdef PERSPICILLUM_WITH_EIGEN
/// Helper to create EigenVectorData from Eigen vectors
template <typename Derived>
inline std::unique_ptr<DataContainer> make_data(const Eigen::MatrixBase<Derived>& vec,
                                                  const std::string& name = "") {
  return std::make_unique<EigenVectorData>(vec, name);
}
#endif

}  // namespace perspicillum
