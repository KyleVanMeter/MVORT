#include <iostream>

int main() {
  int nx = 1200;
  int ny = 600;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  for (int i = ny - 1; i >= 0; i--) {
    for (int j = 0; j < nx; j++) {
      float red = float(j) / float(nx);
      float green = float(i) / float(ny);
      float blue = 0.2;

      int ir = int(255.99 * red);
      int ig = int(255.99 * green);
      int ib = int(255.99 * blue);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
