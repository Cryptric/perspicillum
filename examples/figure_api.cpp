#include <perspicillum.hpp>
#include <cmath>

int main() {
  // Object-Oriented API - detailed plots
  perspicillum::Figure fig(2, 2);

  // Sine wave
  std::vector<double> x;
  for (double t = 0; t < 2 * M_PI; t += 0.1) {
    x.push_back(t);
  }

  std::vector<double> y_sin, y_cos;
  for (double t : x) {
    y_sin.push_back(std::sin(t));
    y_cos.push_back(std::cos(t));
  }

  // Subplot 1,1: sin
  fig.subplot(1, 1).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x, "x"),
    perspicillum::make_data(y_sin, "sin(x)")
  )).style().set_color(0.2f, 0.6f, 1.0f).set_width(2.0f);
  fig.subplot(1, 1).title() = "Sine";

  // Subplot 1,2: cos
  fig.subplot(1, 2).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x, "x"),
    perspicillum::make_data(y_cos, "cos(x)")
  )).style().set_color(1.0f, 0.6f, 0.2f).set_width(2.0f);
  fig.subplot(1, 2).title() = "Cosine";

  // Subplot 2,1: scatter sin
  fig.subplot(2, 1).add_plot(std::make_unique<perspicillum::ScatterPlot>(
    perspicillum::make_data(x, "x"),
    perspicillum::make_data(y_sin, "sin(x)")
  )).style().set_markersize(6);
  fig.subplot(2, 1).title() = "Scatter Sin";

  // Subplot 2,2: both overlaid
  fig.subplot(2, 2).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x, "x"),
    perspicillum::make_data(y_sin, "sin(x)")
  )).style().set_color(0.2f, 0.6f, 1.0f);
  
  fig.subplot(2, 2).add_plot(std::make_unique<perspicillum::LinePlot>(
    perspicillum::make_data(x, "x"),
    perspicillum::make_data(y_cos, "cos(x)")
  )).style().set_color(1.0f, 0.6f, 0.2f);
  fig.subplot(2, 2).title() = "Both";

  fig.set_title("Multi-Subplot Figure");
  fig.show();

  return 0;
}
