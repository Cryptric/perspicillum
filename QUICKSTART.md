# perspicillum Quick Start Guide

## Installation

```bash
cd /home/irz/Documents/perspicillum
mkdir -p build && cd build
cmake ..
make -j4
```

## Minimal Example

```cpp
#include <perspicillum.hpp>

int main() {
  std::vector<double> x = {0, 1, 2, 3, 4};
  std::vector<double> y = {0, 1, 4, 9, 16};
  
  perspicillum::plot(x, y).show();
}
```

Compile with: `g++ -std=c++20 example.cpp -L build -l perspicillum -I include -lglfw -lGL`

## Common Patterns

### 1. Quick Plot (Express API)
```cpp
perspicillum::plot(x, y)
  .title("My Plot")
  .name("Series 1")
  .color(0.2f, 0.6f, 1.0f)
  .width(2.0f)
  .show();
```

### 2. Multiple Plots on Grid
```cpp
perspicillum::Figure fig(2, 2);

fig.subplot(1, 1).add_plot(
  std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x1), perspicillum::make_data(y1)
  )
);

fig.subplot(1, 2).add_plot(
  std::make_unique<perspicillum::ScatterPlot>(
    perspicillum::make_data(x2), perspicillum::make_data(y2)
  )
);

fig.show();
```

### 3. Scatter Plot
```cpp
perspicillum::scatter(x, y)
  .markersize(6)
  .color(1.0f, 0.2f, 0.5f)
  .show();
```

### 4. Bar Chart
```cpp
perspicillum::bar(categories, values)
  .title("Statistics")
  .show();
```

### 5. Using Eigen Vectors
```cpp
#include <Eigen/Dense>

Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(100, 0, 2*M_PI);
Eigen::VectorXd y = x.array().sin();

perspicillum::plot(x, y).show();
```

### 6. Embed in ImGui Window
```cpp
ImGui::Begin("My Plot");
figure.render();  // Don't call .show()
ImGui::End();
```

## Available Methods

### Plot Expression Methods
```cpp
expr.title(const std::string& t)      // Window title
expr.name(const std::string& n)       // Legend label
expr.color(r, g, b, a)                // RGBA: 0-1 range
expr.width(float w)                   // Line width (pixels)
expr.markersize(int s)                // Marker radius (pixels)
expr.show()                           // Display window
```

### Figure Methods
```cpp
Figure fig(rows, cols)                // Create grid
fig.subplot(r, c)                     // Get subplot
fig.add_line(x, y)                    // Add line plot
fig.add_scatter(x, y)                 // Add scatter plot
fig.add_bar(x, y)                     // Add bar plot
fig.set_title(t)                      // Set title
fig.set_size(w, h)                    // Set size
fig.show()                            // Display
fig.render()                          // Render (no window)
```

### PlotStyle Methods
```cpp
style.set_color(r, g, b, a)           // Line/fill color
style.set_name(label)                 // Legend label
style.set_width(w)                    // Line width
style.set_markersize(s)               // Marker size
style.set_showlegend(bool)            // Show in legend
```

## Data Input

```cpp
// Vector of doubles
auto data = perspicillum::make_data(std::vector<double>{1, 2, 3});

// With label
auto data = perspicillum::make_data(my_vec, "label");

// Span (zero-copy)
auto data = perspicillum::make_data(std::span<const double>(vec));

// Eigen vector
Eigen::VectorXd vec = ...;
auto data = perspicillum::make_data(vec);
```

## Color Codes (0-1 RGB)

| Color      | R   | G   | B   |
|-----------|-----|-----|-----|
| Red       | 1.0 | 0.0 | 0.0 |
| Green     | 0.0 | 1.0 | 0.0 |
| Blue      | 0.2 | 0.6 | 1.0 |
| Orange    | 1.0 | 0.6 | 0.2 |
| Purple    | 0.8 | 0.2 | 0.8 |
| Gray      | 0.5 | 0.5 | 0.5 |

## Examples

Run built examples:
```bash
cd /home/irz/Documents/perspicillum/build
./examples/quick_plot       # Express API
./examples/figure_api       # Subplots and OOP
./examples/data_types       # Different data types
```

## Troubleshooting

**Plot window won't show:**
- Check that X11 display is available
- Ensure OpenGL is working: `glxinfo`

**Compilation errors:**
- Use C++20: `g++ -std=c++20`
- Link all libraries: `-lglfw -lGL -lm`

**Data appears empty:**
- Ensure data sizes match: x.size() == y.size()
- Check data values aren't NaN or infinite

## Next Steps

1. Read the full README.md for detailed documentation
2. Study the examples in `examples/`
3. Look at tests in `tests/` for usage patterns
4. Check include files for API details

## Tips & Tricks

- **Chainable API**: All styling methods return `&style`, allowing `method().color().width()`
- **Move semantics**: Pass data with `std::move()` for efficiency
- **Zero-copy**: Use `std::span` for existing data buffers
- **Eigen integration**: Works with any Eigen vector type automatically
- **Window management**: Calling `.show()` blocks until window closes

## Integration Checklist

- [ ] Add `perspicillum` to your CMakeLists.txt: `add_subdirectory(perspicillum)`
- [ ] Link library: `target_link_libraries(your_app perspicillum)`
- [ ] Include header: `#include <perspicillum.hpp>`
- [ ] Use C++20 standard
- [ ] Prepare data as vectors or Eigen types
- [ ] Call `.show()` or `.render()` to display

That's it! You're ready to plot!
