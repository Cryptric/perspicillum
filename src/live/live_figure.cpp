#include "perspicillum/live_figure.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <new>

#include "live_shared.hpp"

namespace perspicillum {

using detail::ShmRegion;
using detail::ShmPlotSlot;
using detail::kShmMaxPlots;

// ---------------------------------------------------------------------------
// LiveSubplot::render  (called from the child process render loop)
// ---------------------------------------------------------------------------

void LiveSubplot::render() {
  if (ImPlot::BeginPlot(
          title_.empty() ? "##live_plot" : title_.c_str(),
          ImVec2(-1, -1),
          ImPlotFlags_None)) {
    if (x_range_.set)
      ImPlot::SetupAxisLimits(ImAxis_X1, x_range_.lo, x_range_.hi,
                              x_range_.locked ? ImPlotCond_Always : ImPlotCond_Once);
    if (y_range_.set)
      ImPlot::SetupAxisLimits(ImAxis_Y1, y_range_.lo, y_range_.hi,
                              y_range_.locked ? ImPlotCond_Always : ImPlotCond_Once);
    for (auto& plot : plots_) {
      plot->render();
    }
    ImPlot::EndPlot();
  }
}

// ---------------------------------------------------------------------------
// LiveFigure::show  — create shared memory, initialise slots, fork
// ---------------------------------------------------------------------------

void LiveFigure::show() {
  if (running_.load())
    throw std::logic_error("LiveFigure::show() called while already running");

  // Count total plots across all subplots.
  int total_plots = 0;
  for (auto& sp : subplots_)
    total_plots += static_cast<int>(sp.plots_.size());

  if (total_plots > kShmMaxPlots)
    throw std::runtime_error(
        "LiveFigure: too many plots (max " + std::to_string(kShmMaxPlots) + ")");

  // Allocate the shared-memory region.
  shm_size_ = sizeof(ShmRegion);
  void* raw = mmap(nullptr, shm_size_,
                   PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS,
                   -1, 0);
  if (raw == MAP_FAILED)
    throw std::runtime_error("LiveFigure: mmap() failed");

  // Placement-new to initialise atomics and POD members.
  ShmRegion* shm = new (raw) ShmRegion();
  shm->num_plots = total_plots;
  shm_region_ = shm;

  // Initialise each plot slot with a process-shared mutex and current data.
  int slot_idx = 0;
  for (auto& sp : subplots_) {
    for (auto& plot : sp.plots_) {
      ShmPlotSlot& slot = shm->plots[slot_idx];

      pthread_mutexattr_t attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
      pthread_mutex_init(&slot.mutex, &attr);
      pthread_mutexattr_destroy(&attr);

      // bind_shm copies the plot's current data into the slot and stores the
      // pointer so future update()/append() calls go straight to shared memory.
      plot->bind_shm(&slot);
      ++slot_idx;
    }
  }

  running_.store(true);

  pid_t pid = fork();
  if (pid < 0) {
    running_.store(false);
    cleanup_shm();
    throw std::runtime_error("LiveFigure: fork() failed");
  }

  if (pid == 0) {
    // ---- child process ----
    // render_loop() owns the GUI; _exit() avoids running parent's atexit/dtors.
    try {
      render_loop();
    } catch (...) {}
    _exit(0);
  }

  // ---- parent process ----
  child_pid_ = pid;
}

// ---------------------------------------------------------------------------
// LiveFigure::close / wait / is_open
// ---------------------------------------------------------------------------

void LiveFigure::close() {
  if (shm_region_)
    static_cast<ShmRegion*>(shm_region_)->close_requested.store(1);
}

void LiveFigure::wait() {
  if (child_pid_ > 0) {
    waitpid(child_pid_, nullptr, 0);
    child_pid_ = -1;
    cleanup_shm();
    running_.store(false);
  }
}

bool LiveFigure::is_open() const {
  if (!running_.load() || child_pid_ <= 0) return false;
  int status = 0;
  pid_t r = waitpid(child_pid_, &status, WNOHANG);
  if (r == 0) return true;   // child still running
  // Child has exited.
  child_pid_ = -1;
  const_cast<LiveFigure*>(this)->cleanup_shm();
  running_.store(false);
  return false;
}

// ---------------------------------------------------------------------------
// Shared-memory cleanup  (parent only, called after child has exited)
// ---------------------------------------------------------------------------

void LiveFigure::cleanup_shm() noexcept {
  if (!shm_region_) return;
  auto* shm = static_cast<ShmRegion*>(shm_region_);
  for (int i = 0; i < shm->num_plots; ++i)
    pthread_mutex_destroy(&shm->plots[i].mutex);
  shm->~ShmRegion();
  munmap(shm_region_, shm_size_);
  shm_region_ = nullptr;
  shm_size_   = 0;
}

// ---------------------------------------------------------------------------
// render_loop  — runs entirely inside the child process
// ---------------------------------------------------------------------------

void LiveFigure::render_loop() {
  auto* shm = static_cast<ShmRegion*>(shm_region_);
  shm->child_alive.store(1);

  if (!glfwInit()) {
    shm->child_alive.store(0);
    return;
  }

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow* window = glfwCreateWindow(
      width_, height_,
      title_.empty() ? "Live Figure" : title_.c_str(),
      nullptr, nullptr);

  if (!window) {
    glfwTerminate();
    shm->child_alive.store(0);
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // vsync

  IMGUI_CHECKVERSION();
  ImGuiContext*  imgui_ctx  = ImGui::CreateContext();
  ImPlotContext* implot_ctx = ImPlot::CreateContext();
  ImGui::SetCurrentContext(imgui_ctx);
  ImPlot::SetCurrentContext(implot_ctx);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImPlot::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // --- event loop ---
  while (!glfwWindowShouldClose(window) &&
         !shm->close_requested.load()) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowBgAlpha(1.0f);
    constexpr ImGuiWindowFlags kFullscreen =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("##live_figure", nullptr, kFullscreen);
    render_frame();
    ImGui::End();

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // --- cleanup ---
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext(implot_ctx);
  ImGui::DestroyContext(imgui_ctx);
  glfwDestroyWindow(window);
  glfwTerminate();

  shm->child_alive.store(0);
  // The mmap is inherited from the parent; the kernel will unmap it on _exit.
}

// ---------------------------------------------------------------------------
// render_frame  — grid / single subplot dispatch (child process)
// ---------------------------------------------------------------------------

void LiveFigure::render_frame() {
  if (rows_ == 1 && cols_ == 1) {
    subplots_[0].render();
    return;
  }

  ImVec2 available  = ImGui::GetContentRegionAvail();
  float  subplot_w  = available.x / static_cast<float>(cols_);
  float  subplot_h  = available.y / static_cast<float>(rows_);

  for (int row = 0; row < rows_; ++row) {
    for (int col = 0; col < cols_; ++col) {
      if (col > 0) ImGui::SameLine();

      ImGui::BeginChild(
          ImGui::GetID(reinterpret_cast<void*>(
              static_cast<uintptr_t>(row * cols_ + col))),
          ImVec2(subplot_w, subplot_h),
          false);

      int idx = row * cols_ + col;
      if (idx < static_cast<int>(subplots_.size()))
        subplots_[idx].render();

      ImGui::EndChild();
    }
  }
}

}  // namespace perspicillum
