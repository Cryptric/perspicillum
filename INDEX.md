# perspicillum - Complete Project Index

## 📋 Documentation

| File | Purpose | Read Time |
|------|---------|-----------|
| **README.md** | Complete API reference, architecture, design | 15 min |
| **QUICKSTART.md** | Quick reference for common tasks | 5 min |
| **GETTING_STARTED.txt** | Tutorials and minimal examples | 10 min |
| **PROJECT_SUMMARY.txt** | Architecture, statistics, features | 10 min |
| **INDEX.md** | This file - project navigation | 2 min |
| **LICENSE** | MIT License | 1 min |

## 🏗️ Source Code Structure

### Core Library
- **include/perspicillum.hpp** - Main include file (use this)
- **include/perspicillum/perspicillum.hpp** - Aggregator header
- **include/perspicillum/data_container.hpp** - Data abstraction layer
- **include/perspicillum/plot_style.hpp** - Styling attributes (Plotly-like)
- **include/perspicillum/plot_base.hpp** - Abstract plot interface
- **include/perspicillum/plots.hpp** - LinePlot, ScatterPlot, BarPlot classes
- **include/perspicillum/figure.hpp** - Figure and Subplot classes
- **include/perspicillum/window_manager.hpp** - Window lifecycle management
- **include/perspicillum/express.hpp** - Quick plotting API

### Implementation
- **src/backends/imgui_backend.cpp** - GLFW/ImGui/ImPlot initialization
- **src/backends/implot_adapter.cpp** - Plot rendering to implot
- **src/core/figure.cpp** - Figure rendering logic

### Examples
- **examples/quick_plot.cpp** - Express API demo (simplest)
- **examples/figure_api.cpp** - Object-oriented API with subplots
- **examples/data_types.cpp** - Eigen vector support

### Tests
- **tests/data_container_test.cpp** - Data type unit tests

### Build Configuration
- **CMakeLists.txt** - Root CMake configuration
- **examples/CMakeLists.txt** - Example build targets
- **tests/CMakeLists.txt** - Test build target

### External Dependencies (included)
- **external/imgui/** - Dear ImGui (GUI framework)
- **external/implot/** - ImPlot (plotting backend)

## 🚀 Quick Start

```bash
# Build
cd perspicillum && mkdir build && cd build
cmake .. && make -j4

# Test
make test

# Run examples
./examples/quick_plot
./examples/figure_api
./examples/data_types
```

## 📚 API Overview

### Express API (Quick Plotting)
```cpp
#include <perspicillum.hpp>

perspicillum::plot(x, y)
  .title("My Plot")
  .color(0.2f, 0.6f, 1.0f)
  .show();
```

### Object-Oriented API
```cpp
perspicillum::Figure fig(2, 2);
fig.subplot(1, 1).add_plot(
  std::make_unique<perspicillum::LinePlot>(x, y)
);
fig.show();
```

### Data Types
- std::vector<double>
- std::span<double> (zero-copy)
- Eigen::VectorXd (with PERSPICILLUM_WITH_EIGEN=ON)

### Plot Types
- LinePlot
- ScatterPlot
- BarPlot

## 🎨 Styling

```cpp
// Colors (RGBA: 0-1 range)
.color(1.0f, 0.0f, 0.0f)        // Red
.color(0.0f, 1.0f, 0.0f)        // Green
.color(0.0f, 0.0f, 1.0f)        // Blue

// Line styling
.width(2.0f)                      // Line width in pixels
.markersize(6)                    // Marker radius in pixels
.name("Series Label")             // Legend name
.title("Figure Title")            // Window/figure title
```

## 📖 Where to Find Things

### I want to...

**...plot data quickly**
→ README.md "Quick Start" + QUICKSTART.md

**...understand the API**
→ README.md "API Reference" + header files

**...see working code**
→ examples/ directory

**...integrate into my project**
→ GETTING_STARTED.txt "Use In Your Project" + README.md

**...understand the architecture**
→ PROJECT_SUMMARY.txt + README.md "Architecture"

**...troubleshoot issues**
→ GETTING_STARTED.txt "Troubleshooting" + README.md

**...use Eigen vectors**
→ examples/data_types.cpp + README.md "Data Types"

**...embed in ImGui**
→ README.md "Integration with Existing GUIs" + examples

**...customize plots**
→ QUICKSTART.md "Available Methods" + examples

## 🔧 Configuration Options

### Build Options
```bash
# Enable/disable features
cmake .. \
  -DPERSPICILLUM_BUILD_EXAMPLES=ON       # Build examples
  -DPERSPICILLUM_BUILD_TESTS=ON          # Build tests
  -DPERSPICILLUM_WITH_EIGEN=ON           # Eigen support
  -DCMAKE_BUILD_TYPE=Release             # Release build
```

### C++ Standard
- Requires: C++20
- Set in CMakeLists.txt or via: `-std=c++20`

## 📦 Deliverables Checklist

- ✓ Production-ready C++20 code
- ✓ Two-tier API (Express + OOP)
- ✓ 3 plot types (Line, Scatter, Bar)
- ✓ Multiple data type support
- ✓ Multi-subplot support
- ✓ Window management
- ✓ GUI embedding capability
- ✓ Unit tests (all passing)
- ✓ 3 working examples
- ✓ CMake build system
- ✓ Comprehensive documentation
- ✓ MIT License

## 🎯 Key Features

| Feature | Status | Details |
|---------|--------|---------|
| Quick plotting | ✓ | One-liner with `plot(x, y).show()` |
| Multi-subplot | ✓ | `Figure(rows, cols)` |
| Data flexibility | ✓ | Vector, Span, Eigen |
| Styling | ✓ | Color, width, markers, labels |
| GUI embedding | ✓ | `.render()` without window |
| Type safety | ✓ | C++20 templates |
| Performance | ✓ | Zero-copy with span |
| Documentation | ✓ | 40+ KB of docs |

## 🏆 Code Quality

- **Lines of Code**: ~1,850 lines
- **Headers**: ~800 lines
- **Implementation**: ~400 lines
- **Tests**: All passing ✓
- **Compiler Warnings**: None ✓
- **C++ Standard**: C++20 compliant ✓
- **Smart Pointers**: Throughout, no raw pointers ✓
- **RAII**: Proper resource management ✓

## 🔗 Dependencies

**System Libraries (auto-installed)**:
- build-essential
- cmake
- libglfw3-dev
- libgl1-mesa-dev
- libeigen3-dev
- libglm-dev

**Included in Project**:
- Dear ImGui (GUI framework)
- ImPlot (plotting backend)

## 📝 File Sizes

| Component | Size |
|-----------|------|
| libperspicillum.a | 6.1 MB |
| quick_plot | 3.9 MB |
| figure_api | 3.9 MB |
| data_types | 3.9 MB |
| data_container_test | 19 KB |
| Total documentation | 45 KB |

## 🎓 Learning Path

1. **Start Here**: README.md "Quick Start"
2. **Try Examples**: `./examples/quick_plot`
3. **Learn API**: QUICKSTART.md
4. **Deep Dive**: README.md "API Reference"
5. **Integrate**: GETTING_STARTED.txt "Use In Your Project"
6. **Master**: Study header files and implementation

## 🐛 Troubleshooting

**Build fails**: See GETTING_STARTED.txt "Troubleshooting"
**Plot doesn't show**: Check GETTING_STARTED.txt "Plot doesn't display"
**Compilation errors**: See QUICKSTART.md "Tips & Tricks"
**Integration issues**: See README.md or GETTING_STARTED.txt

## 📞 Quick Reference

| Task | Command/Code |
|------|--------------|
| Build | `cmake .. && make -j4` |
| Test | `make test` |
| Run example | `./examples/quick_plot` |
| Use in code | `#include <perspicillum.hpp>` |
| Quick plot | `perspicillum::plot(x, y).show()` |
| Link in CMake | `target_link_libraries(app perspicillum)` |
| Set C++ standard | `set_property(TARGET app PROPERTY CXX_STANDARD 20)` |

## ✨ What's Special

1. **Plotly-Inspired API** - Familiar syntax from JavaScript
2. **Two-Tier Design** - Both quick and detailed approaches
3. **Modern C++** - C++20, smart pointers, no raw memory
4. **Zero-Copy** - std::span support for efficiency
5. **Extensible** - Easy to add new plot types
6. **Well-Documented** - 40+ KB of documentation
7. **Tested** - Unit tests with 100% pass rate
8. **Production-Ready** - Immediately usable

## 🎉 You're All Set!

Everything is ready to use. Pick a documentation file and start plotting!

**Recommended starting point**: GETTING_STARTED.txt

Happy plotting! 📊
