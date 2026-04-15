#include <perspicillum.hpp>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstdio>

int main() {
  perspicillum::LiveFigure fig(1, 2);
  fig.set_title("Live Plot Example");

  auto& line   = fig.subplot(1, 1).add_line();
  auto& scatter = fig.subplot(1, 2).add_scatter();
  line.style().set_name("sin(t)").set_color(0.2f, 0.8f, 0.4f).set_width(2.0f);
  scatter.style().set_name("Lissajous").set_color(1.0f, 0.5f, 0.1f).set_markersize(3);

  fig.show();
  std::printf("Window open — streaming data. Close the window to quit.\n");

  double t = 0.0;
  std::vector<double> tx, ty, sx, sy;

  while (fig.is_open()) {
    t += 0.05;

    // rolling 200-point sine wave
    tx.push_back(t);
    ty.push_back(std::sin(t));
    if (tx.size() > 200) { tx.erase(tx.begin()); ty.erase(ty.begin()); }
    line.update(tx, ty);

    // Lissajous figure
    double phase = t * 0.1;
    sx.push_back(std::sin(3.0 * t + phase));
    sy.push_back(std::sin(2.0 * t));
    if (sx.size() > 500) { sx.erase(sx.begin()); sy.erase(sy.begin()); }
    scatter.update(sx, sy);

    std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 Hz
  }

  fig.wait();
  std::printf("Window closed.\n");
  return 0;
}
