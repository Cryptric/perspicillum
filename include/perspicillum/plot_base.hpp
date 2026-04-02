#pragma once

#include <memory>
#include "data_container.hpp"
#include "plot_style.hpp"

namespace perspicillum {

class Figure;

/// Base class for all plot types
class PlotBase {
public:
  virtual ~PlotBase() = default;
  
  /// Render this plot in the current ImPlot context
  virtual void render() = 0;
  
  /// Get the plot's style
  virtual PlotStyle& style() = 0;
  virtual const PlotStyle& style() const = 0;
};

}  // namespace perspicillum
