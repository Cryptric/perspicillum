#include <perspicillum.hpp>
#include <cassert>
#include <iostream>

void test_vector_data() {
  std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
  auto container = perspicillum::make_data(data, "test");
  
  assert(container->size() == 5);
  assert(container->name() == std::string("test"));
  for (size_t i = 0; i < 5; ++i) {
    assert(container->data()[i] == data[i]);
  }
  std::cout << "✓ Vector data test passed\n";
}

void test_span_data() {
  std::vector<double> data = {1.0, 2.0, 3.0};
  auto container = perspicillum::make_data(std::span<const double>(data));
  
  assert(container->size() == 3);
  for (size_t i = 0; i < 3; ++i) {
    assert(container->data()[i] == data[i]);
  }
  std::cout << "✓ Span data test passed\n";
}

#ifdef PERSPICILLUM_WITH_EIGEN
void test_eigen_data() {
  Eigen::VectorXd vec = Eigen::VectorXd::LinSpaced(5, 1.0, 5.0);
  auto container = perspicillum::make_data(vec, "eigen");
  
  assert(container->size() == 5);
  assert(container->name() == std::string("eigen"));
  for (size_t i = 0; i < 5; ++i) {
    assert(container->data()[i] == vec(i));
  }
  std::cout << "✓ Eigen data test passed\n";
}
#endif

int main() {
  std::cout << "Running data container tests...\n";
  
  test_vector_data();
  test_span_data();
  
#ifdef PERSPICILLUM_WITH_EIGEN
  test_eigen_data();
#endif
  
  std::cout << "All tests passed!\n";
  return 0;
}
