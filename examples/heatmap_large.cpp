#include <perspicillum.hpp>
#include <cmath>
#include <vector>

int main() {
  constexpr int N = 2048;
  std::vector<double> z(N * N);
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      double x = (c - N / 2.0) / 200.0;
      double y = (r - N / 2.0) / 200.0;
      z[r * N + c] = std::sin(x * x + y * y);
    }
  }

  perspicillum::heatmap(z, N, N)
      .title("2048x2048 Heatmap")
      .colormap(perspicillum::Colormap::Viridis)
      .scale(-1.0, 1.0)
      .show();

  return 0;
}
