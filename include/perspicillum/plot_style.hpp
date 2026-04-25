#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace perspicillum {

enum class Colormap {
  Viridis,
  Plasma,
  Hot,
  Cool,
  Jet,
  Greys,
  RdBu,
  BrBG,
  PiYG,
  Spectral,
};

/// Plotly-style attributes for plots
struct PlotStyle {
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  std::string name;
  bool showlegend{true};
  float width{1.0f};
  int markersize{4};
  
  PlotStyle& set_color(float r, float g, float b, float a = 1.0f) {
    color = {r, g, b, a};
    return *this;
  }
  
  PlotStyle& set_name(const std::string& n) {
    name = n;
    return *this;
  }
  
  PlotStyle& set_showlegend(bool show) {
    showlegend = show;
    return *this;
  }
  
  PlotStyle& set_width(float w) {
    width = w;
    return *this;
  }
  
  PlotStyle& set_markersize(int size) {
    markersize = size;
    return *this;
  }
};

}  // namespace perspicillum
