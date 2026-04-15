#include "perspicillum/live_plots.hpp"

#include <algorithm>
#include <imgui.h>
#include <implot.h>

#include "live_shared.hpp"

namespace perspicillum {

using detail::ShmPlotSlot;
using detail::kShmMaxPoints;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

namespace {

// Lock the slot's process-shared mutex, copy x/y into the slot, unlock.
void shm_update(ShmPlotSlot* slot,
                const double* x, const double* y, int n) {
  n = std::min(n, kShmMaxPoints);
  pthread_mutex_lock(&slot->mutex);
  std::copy_n(x, n, slot->x);
  std::copy_n(y, n, slot->y);
  slot->num_points = n;
  pthread_mutex_unlock(&slot->mutex);
}

void shm_append(ShmPlotSlot* slot, double x, double y) {
  pthread_mutex_lock(&slot->mutex);
  if (slot->num_points < kShmMaxPoints) {
    slot->x[slot->num_points] = x;
    slot->y[slot->num_points] = y;
    ++slot->num_points;
  }
  pthread_mutex_unlock(&slot->mutex);
}

}  // namespace

// ---------------------------------------------------------------------------
// LiveScatter
// ---------------------------------------------------------------------------

LiveScatter::LiveScatter(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveScatter::LiveScatter(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveScatter::bind_shm(void* slot) {
  auto* s = static_cast<ShmPlotSlot*>(slot);
  // Copy current data into the slot (mutex already initialised by LiveFigure).
  shm_update(s, x_.data(), y_.data(), static_cast<int>(x_.size()));
  shm_slot_ = slot;
  // Release local storage — data now lives in shared memory.
  x_.clear();  x_.shrink_to_fit();
  y_.clear();  y_.shrink_to_fit();
}

void LiveScatter::update(const std::vector<double>& x, const std::vector<double>& y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_ = x;
    y_ = y;
  }
}

void LiveScatter::update(std::span<const double> x, std::span<const double> y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_.assign(x.begin(), x.end());
    y_.assign(y.begin(), y.end());
  }
}

void LiveScatter::append(double x, double y) {
  if (shm_slot_) {
    shm_append(static_cast<ShmPlotSlot*>(shm_slot_), x, y);
  } else {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }
}

void LiveScatter::render() {
  if (shm_slot_) {
    auto* slot = static_cast<ShmPlotSlot*>(shm_slot_);
    pthread_mutex_lock(&slot->mutex);
    int n = slot->num_points;
    if (n == 0) { pthread_mutex_unlock(&slot->mutex); return; }

    ImPlotSpec spec;
    spec.Marker = ImPlotMarker_Circle;
    spec.MarkerSize = static_cast<float>(style_.markersize);
    spec.MarkerFillColor =
        ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    ImPlot::PlotScatter(
        style_.name.empty() ? "Scatter" : style_.name.c_str(),
        slot->x, slot->y, n, spec);
    pthread_mutex_unlock(&slot->mutex);
  } else {
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
}

// ---------------------------------------------------------------------------
// LiveLine
// ---------------------------------------------------------------------------

LiveLine::LiveLine(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveLine::LiveLine(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveLine::bind_shm(void* slot) {
  auto* s = static_cast<ShmPlotSlot*>(slot);
  shm_update(s, x_.data(), y_.data(), static_cast<int>(x_.size()));
  shm_slot_ = slot;
  x_.clear();  x_.shrink_to_fit();
  y_.clear();  y_.shrink_to_fit();
}

void LiveLine::update(const std::vector<double>& x, const std::vector<double>& y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_ = x;
    y_ = y;
  }
}

void LiveLine::update(std::span<const double> x, std::span<const double> y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_.assign(x.begin(), x.end());
    y_.assign(y.begin(), y.end());
  }
}

void LiveLine::append(double x, double y) {
  if (shm_slot_) {
    shm_append(static_cast<ShmPlotSlot*>(shm_slot_), x, y);
  } else {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }
}

void LiveLine::render() {
  if (shm_slot_) {
    auto* slot = static_cast<ShmPlotSlot*>(shm_slot_);
    pthread_mutex_lock(&slot->mutex);
    int n = slot->num_points;
    if (n == 0) { pthread_mutex_unlock(&slot->mutex); return; }

    ImPlotSpec spec;
    spec.LineColor =
        ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    spec.LineWeight = style_.width;
    ImPlot::PlotLine(
        style_.name.empty() ? "Line" : style_.name.c_str(),
        slot->x, slot->y, n, spec);
    pthread_mutex_unlock(&slot->mutex);
  } else {
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
}

// ---------------------------------------------------------------------------
// LiveBar
// ---------------------------------------------------------------------------

LiveBar::LiveBar(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {}

LiveBar::LiveBar(std::span<const double> x, std::span<const double> y)
    : x_(x.begin(), x.end()), y_(y.begin(), y.end()) {}

void LiveBar::bind_shm(void* slot) {
  auto* s = static_cast<ShmPlotSlot*>(slot);
  shm_update(s, x_.data(), y_.data(), static_cast<int>(x_.size()));
  shm_slot_ = slot;
  x_.clear();  x_.shrink_to_fit();
  y_.clear();  y_.shrink_to_fit();
}

void LiveBar::update(const std::vector<double>& x, const std::vector<double>& y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_ = x;
    y_ = y;
  }
}

void LiveBar::update(std::span<const double> x, std::span<const double> y) {
  if (shm_slot_) {
    shm_update(static_cast<ShmPlotSlot*>(shm_slot_),
               x.data(), y.data(), static_cast<int>(x.size()));
  } else {
    std::lock_guard lock(mutex_);
    x_.assign(x.begin(), x.end());
    y_.assign(y.begin(), y.end());
  }
}

void LiveBar::append(double x, double y) {
  if (shm_slot_) {
    shm_append(static_cast<ShmPlotSlot*>(shm_slot_), x, y);
  } else {
    std::lock_guard lock(mutex_);
    x_.push_back(x);
    y_.push_back(y);
  }
}

void LiveBar::render() {
  if (shm_slot_) {
    auto* slot = static_cast<ShmPlotSlot*>(shm_slot_);
    pthread_mutex_lock(&slot->mutex);
    int n = slot->num_points;
    if (n == 0) { pthread_mutex_unlock(&slot->mutex); return; }

    ImPlotSpec spec;
    spec.FillColor =
        ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    ImPlot::PlotBars(
        style_.name.empty() ? "Bar" : style_.name.c_str(),
        slot->x, slot->y, n, 0.8, spec);
    pthread_mutex_unlock(&slot->mutex);
  } else {
    std::lock_guard lock(mutex_);
    if (x_.empty() || x_.size() != y_.size()) return;

    ImPlotSpec spec;
    spec.FillColor =
        ImVec4(style_.color.x, style_.color.y, style_.color.z, style_.color.w);
    ImPlot::PlotBars(
        style_.name.empty() ? "Bar" : style_.name.c_str(),
        x_.data(), y_.data(), static_cast<int>(x_.size()), 0.8, spec);
  }
}

}  // namespace perspicillum
