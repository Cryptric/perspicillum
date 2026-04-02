#include <perspicillum.hpp>
#include <cmath>

int main() {
  // Express API - quick plotting
  std::vector<double> x, y;
  
  for (double t = 0; t < 2 * M_PI; t += 0.1) {
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
