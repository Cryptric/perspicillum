#include <perspicillum.hpp>
#include <cmath>
#include <vector>

int main() {
  // Express API: quick single heatmap
  {
    // 2D Gaussian on a 40x40 grid
    constexpr int N = 40;
    std::vector<double> z(N * N);
    for (int r = 0; r < N; ++r) {
      for (int c = 0; c < N; ++c) {
        double x = (c - N / 2.0) / 6.0;
        double y = (r - N / 2.0) / 6.0;
        z[r * N + c] = std::exp(-(x * x + y * y));
      }
    }

    perspicillum::heatmap(z, N, N)
        .title("2D Gaussian")
        .name("gaussian")
        .colormap(perspicillum::Colormap::Viridis)
        .scale(0.0, 1.0)
        .show();
  }

  // Object-oriented API: two heatmaps side by side
  {
    constexpr int R = 20, C = 30;
    std::vector<double> sin_grid(R * C), jet_grid(R * C);
    for (int r = 0; r < R; ++r) {
      for (int c = 0; c < C; ++c) {
        double x = c * 2.0 * M_PI / C;
        double y = r * 2.0 * M_PI / R;
        sin_grid[r * C + c] = std::sin(x) * std::cos(y);
        jet_grid[r * C + c] = std::sin(x + y);
      }
    }

    perspicillum::Figure fig(1, 2);

    fig.subplot(1, 1).add_plot(std::make_unique<perspicillum::HeatmapPlot>(sin_grid, R, C))
        .set_colormap(perspicillum::Colormap::RdBu)
        .set_scale(-1.0, 1.0)
        .set_show_labels(false);
    fig.subplot(1, 1).title() = "sin(x)cos(y)";

    fig.subplot(1, 2).add_plot(std::make_unique<perspicillum::HeatmapPlot>(jet_grid, R, C))
        .set_colormap(perspicillum::Colormap::Jet)
        .set_scale(-1.0, 1.0);
    fig.subplot(1, 2).title() = "sin(x+y)";

    fig.set_title("Heatmap Demo");
    fig.show();
  }

#ifdef PERSPICILLUM_WITH_EIGEN
  // Eigen matrix heatmap
  {
    Eigen::MatrixXd M(15, 15);
    for (int i = 0; i < 15; ++i)
      for (int j = 0; j < 15; ++j)
        M(i, j) = std::sin(i * 0.4) * std::cos(j * 0.4);

    perspicillum::heatmap(M)
        .title("Eigen Matrix Heatmap")
        .colormap(perspicillum::Colormap::Plasma)
        .show();
  }
#endif

  return 0;
}
