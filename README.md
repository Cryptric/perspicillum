# perspicillum - C++ Plotting Library

A simple, elegant C++ plotting library inspired by Plotly's API, leveraging the powerful implot library for rendering.

## Features

- **Easy-to-use plotting**: Quick plotting with intuitive express-style API
- **Flexible data support**: Works with `std::vector<double>` and Eigen matrices
- **Two-tier API**:
  - **Express API** for quick plots: `plot(x, y).show()`
  - **Object-oriented API** for detailed control and GUI embedding
- **Multiple plot types**: Line, Scatter, Bar, and extensible framework for more
- **Window management**: Automatic window handling or embed in existing GUIs
- **Plotly-inspired styling**: Familiar attribute names and chainable API

## Installation

### Prerequisites

- C++20 compiler
- CMake 3.20 or higher
- Development libraries: glfw3, OpenGL, Eigen3 (optional)

### Build

```bash
cd perspicillum
mkdir build && cd build
cmake ..
make -j4
```

### Optional: Eigen Support

Eigen support is enabled by default. Disable with:
```bash
cmake .. -DPERSPICILLUM_WITH_EIGEN=OFF
```

## Quick Start

### Express API - Quick Plotting

```cpp
#include <perspicillum.hpp>
#include <vector>
#include <cmath>

int main() {
  std::vector<double> x, y;
  for (double t = 0; t < 6.28; t += 0.1) {
    x.push_back(t);
    y.push_back(std::sin(t));
  }

  perspicillum::plot(x, y)
    .title("Sine Wave")
    .name("sin(x)")
    .color(0.2f, 0.6f, 1.0f)
    .width(2.0f)
    .show();

  return 0;
}
```

### Object-Oriented API - Detailed Control

```cpp
#include <perspicillum.hpp>
#include <vector>
#include <cmath>

int main() {
  std::vector<double> x;
  for (double t = 0; t < 6.28; t += 0.1) {
    x.push_back(t);
  }

  std::vector<double> y_sin, y_cos;
  for (double t : x) {
    y_sin.push_back(std::sin(t));
    y_cos.push_back(std::cos(t));
  }

  // Create a 2x2 subplot grid
  perspicillum::Figure fig(2, 2);

  // Sine plot
  fig.subplot(1, 1).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x),
    perspicillum::make_data(y_sin)
  )).style().set_color(0.2f, 0.6f, 1.0f);
  fig.subplot(1, 1).title() = "Sine";

  // Cosine plot
  fig.subplot(1, 2).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x),
    perspicillum::make_data(y_cos)
  )).style().set_color(1.0f, 0.6f, 0.2f);
  fig.subplot(1, 2).title() = "Cosine";

  fig.set_title("Trigonometric Functions");
  fig.show();

  return 0;
}
```

### Using Eigen Vectors

```cpp
#include <perspicillum.hpp>
#include <Eigen/Dense>
#include <cmath>

int main() {
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(100, 0, 2*M_PI);
  Eigen::VectorXd y = x.array().sin();

  perspicillum::scatter(x, y)
    .title("Scatter Plot from Eigen")
    .name("sin(x)")
    .markersize(5)
    .color(1.0f, 0.2f, 0.5f)
    .show();

  return 0;
}
```

## API Reference

### Express API Functions

```cpp
// Create plots with std::vector<double>
perspicillum::plot(const std::vector<double>& x, const std::vector<double>& y);
perspicillum::scatter(const std::vector<double>& x, const std::vector<double>& y);
perspicillum::bar(const std::vector<double>& x, const std::vector<double>& y);

// Or with DataContainer pointers for more control
perspicillum::plot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y);
```

### PlotExpression (Return Type of Express Functions)

```cpp
// Chainable styling methods
expr.title(const std::string& t)        // Set window/figure title
expr.name(const std::string& n)         // Set plot label
expr.color(float r, float g, float b, float a = 1.0f)  // RGBA: 0-1 range
expr.width(float w)                     // Line width in pixels
expr.markersize(int size)               // Marker radius in pixels
expr.show()                             // Display the plot window
```

### Figure (Object-Oriented API)

```cpp
perspicillum::Figure fig;                    // Single subplot
perspicillum::Figure fig(rows, cols);        // Subplot grid

fig.subplot(row, col)                        // Get subplot (1-indexed)
fig.add_line(x_data, y_data)                // Add line plot
fig.add_scatter(x_data, y_data)             // Add scatter plot
fig.add_bar(x_data, y_data)                 // Add bar plot
fig.set_title(const std::string& t)         // Set figure title
fig.set_size(width, height)                 // Set window size
fig.show()                                   // Display in window
fig.render()                                 // Render to current context
```

### Plot Objects

```cpp
perspicillum::LinePlot(x_data, y_data)
perspicillum::ScatterPlot(x_data, y_data)
perspicillum::BarPlot(x_data, y_data)

// All support:
plot.style().set_color(r, g, b, a)
plot.style().set_name(label)
plot.style().set_width(w)
plot.style().set_markersize(s)
plot.render()                        // Render to ImPlot context
```

### Data Containers

```cpp
// Create from std::vector<double>
auto data = perspicillum::make_data(std::vector<double>{1, 2, 3});
auto data = perspicillum::make_data(std::vector<double>{1, 2, 3}, "label");

// Create from span
auto data = perspicillum::make_data(std::span<const double>(vec));

// Create from Eigen vectors (if PERSPICILLUM_WITH_EIGEN=ON)
auto data = perspicillum::make_data(eigen_vector);
auto data = perspicillum::make_data(eigen_vector, "label");
```

## Data Types

### Supported Input Types

- **std::vector<double>**: Native C++ vector
- **std::span<double>**: View into contiguous data
- **Eigen::VectorXd**: Eigen column vectors
- **Eigen::RowVectorXd**: Eigen row vectors
- Custom types via DataContainer interface

## Architecture

### Two-Tier Design

**Express API** (`perspicillum/express.hpp`):
- Fast, simple interface for quick plotting
- Functions like `plot()`, `scatter()`, `bar()`
- Returns chainable `PlotExpression` objects
- Automatically creates window and renders

**Object-Oriented API** (`perspicillum/figure.hpp`):
- Detailed control over plots and layout
- `Figure` and `Subplot` classes
- Support for multiple subplots
- Can be embedded in existing ImGui contexts

### Rendering Pipeline

1. **Data Containers** - Abstracts different data sources
2. **Plot Objects** - Encapsulate plot logic and styling
3. **Subplots** - Organize multiple plots
4. **Figure** - Manages layout and coordinate systems
5. **Window Manager** - Handles GLFW/ImGui/ImPlot lifecycle
6. **Rendering** - ImPlot draws to OpenGL context

## Building with Your Project

### CMake Integration

```cmake
add_subdirectory(perspicillum)
target_link_libraries(my_app perspicillum)
```

### Include

```cpp
#include <perspicillum.hpp>
```

All public types and functions are in the `perspicillum` namespace.

## Examples

Build examples with:
```bash
cmake .. -DPERSPICILLUM_BUILD_EXAMPLES=ON
make
```

Run examples:
```bash
./examples/quick_plot          # Express API demo
./examples/figure_api          # Object-oriented API demo  
./examples/data_types          # Multiple data type demo
```

## Testing

```bash
make test
# or
./tests/data_container_test
```

## Dependencies

### Required
- **implot** - Plotting backend (included as git submodule)
- **imgui** - UI framework (included as git submodule)
- **glfw3** - Window creation
- **OpenGL** - Graphics rendering

### Optional
- **Eigen3** - For vector/matrix data types

## Implementation Notes

- Uses C++20 features (concepts, spans, etc.)
- Header-only core classes where possible
- Move semantics for performance
- Zero-copy data handling with span support
- Normalized color values (0-1 range, matching Plotly)

## Extending the Library

### Adding New Plot Types

Create a new class inheriting from `PlotBase`:

```cpp
class MyPlot : public perspicillum::PlotBase {
private:
  std::unique_ptr<DataContainer> x_, y_;
  PlotStyle style_;

public:
  MyPlot(std::unique_ptr<DataContainer> x, std::unique_ptr<DataContainer> y)
    : x_(std::move(x)), y_(std::move(y)) {}

  void render() override {
    // Use implot API directly
    ImPlot::PlotLine(...);
  }

  PlotStyle& style() override { return style_; }
  const PlotStyle& style() const override { return style_; }
};
```

## License

MIT License - See LICENSE file for details

## Contributing

Contributions welcome! Please ensure:
- Code follows C++20 standards
- Tests pass
- Examples compile and run
- Documentation is updated

## Troubleshooting

### Build errors with ImPlot

Ensure both `implot/implot.cpp` and `implot/implot_items.cpp` are compiled.

### GLFW/OpenGL errors

Make sure your system has:
- `libglfw3-dev`
- `libgl1-mesa-dev`
- `libxi-dev`
- `libxinerama-dev`
- `libxcursor-dev`

Install with: `apt-get install libglfw3-dev libgl1-mesa-dev`

### Eigen data not working

Ensure Eigen3 is installed: `apt-get install libeigen3-dev`

Build with Eigen support explicitly:
```bash
cmake .. -DPERSPICILLUM_WITH_EIGEN=ON
```

## Credits

- **implot** by Evan Pezent (https://github.com/epezent/implot)
- **imgui** by Omar Cornut (https://github.com/ocornut/imgui)
- **glfw** (https://github.com/glfw/glfw)
- **Plotly** for API inspiration
