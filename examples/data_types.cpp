#include <perspicillum.hpp>
#include <cmath>

#ifdef PERSPICILLUM_WITH_EIGEN
#include <Eigen/Dense>

int main() {
  // Using Eigen vectors for plotting
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(100, 0, 2 * M_PI);
  Eigen::VectorXd y = x.array().sin();

  perspicillum::scatter(x, y)
    .title("Using Eigen Data")
    .name("sin(x) from Eigen")
    .color(1.0f, 0.2f, 0.5f)
    .markersize(5)
    .show();

  return 0;
}
#else
int main() {
  // Fallback without Eigen
  std::vector<double> x, y;
  for (double t = 0; t < 2 * M_PI; t += 0.1) {
    x.push_back(t);
    y.push_back(std::cos(t));
  }

  perspicillum::bar(x, y)
    .title("Bar Chart Example")
    .show();

  return 0;
}
#endif
