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

static ImPlotColormap to_implot_colormap(Colormap cm) {
  switch (cm) {
    case Colormap::Viridis:  return ImPlotColormap_Viridis;
    case Colormap::Plasma:   return ImPlotColormap_Plasma;
    case Colormap::Hot:      return ImPlotColormap_Hot;
    case Colormap::Cool:     return ImPlotColormap_Cool;
    case Colormap::Jet:      return ImPlotColormap_Jet;
    case Colormap::Greys:    return ImPlotColormap_Greys;
    case Colormap::RdBu:     return ImPlotColormap_RdBu;
    case Colormap::BrBG:     return ImPlotColormap_BrBG;
    case Colormap::PiYG:     return ImPlotColormap_PiYG;
    case Colormap::Spectral: return ImPlotColormap_Spectral;
  }
  return ImPlotColormap_Viridis;
}

void HeatmapPlot::render() {
  if (values_.empty()) return;
  ImPlot::PushColormap(to_implot_colormap(colormap_));
  ImPlotSpec spec;
  if (col_major_)
    spec.Flags = ImPlotHeatmapFlags_ColMajor;
  ImPlot::PlotHeatmap(
      style_.name.empty() ? "Heatmap" : style_.name.c_str(),
      raw_data(),
      rows_, cols_,
      scale_min_, scale_max_,
      show_labels_ ? "%.1f" : nullptr,
      ImPlotPoint(0, 0), ImPlotPoint(static_cast<double>(cols_), static_cast<double>(rows_)),
      spec
  );
  ImPlot::PopColormap();
}

}  // namespace perspicillum
