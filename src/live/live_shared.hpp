#pragma once
// Internal shared-memory layout for the live module.
// This header is NOT installed and NOT part of the public API.

#include <atomic>
#include <cstdint>
#include <pthread.h>

namespace perspicillum::detail {

// Compile-time guard: atomic<int32_t> must be lock-free to be safe in a
// POSIX shared-memory mapping that is accessed from two separate processes.
static_assert(std::atomic<int32_t>::is_always_lock_free,
              "perspicillum live module requires lock-free int32 atomics");

// Per-figure tuning knobs.
static constexpr int kShmMaxPlots  = 16;
static constexpr int kShmMaxPoints = 32768;  // ~256 KB x/y per plot

// ---------------------------------------------------------------------------
// One slot per live plot.
// Only the dynamic x/y data lives here; plot type, style, and subplot layout
// are captured from the parent's local objects at fork time (COW).
// ---------------------------------------------------------------------------
struct ShmPlotSlot {
    pthread_mutex_t mutex;              // PTHREAD_PROCESS_SHARED
    int32_t         num_points = 0;
    double          x[kShmMaxPoints];
    double          y[kShmMaxPoints];
};

// ---------------------------------------------------------------------------
// Control block + all plot slots — the entire shared-memory region.
// ---------------------------------------------------------------------------
struct ShmRegion {
    // Control flags — written from either process.
    // Placed on their own cache line to avoid false sharing with plot data.
    alignas(64) std::atomic<int32_t> close_requested{0};  // parent → child
    alignas(64) std::atomic<int32_t> child_alive{0};      // child → parent

    int32_t     num_plots = 0;
    ShmPlotSlot plots[kShmMaxPlots];
};

}  // namespace perspicillum::detail
