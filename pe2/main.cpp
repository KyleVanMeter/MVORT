/*
 * Kyle Van Meter
 * pe1
 * These files implement the Point class, and test them
 */

#include <iostream>
#include "Point.h"

int main(int argc, char* argv[]) {
  Point p, q, r;

  p.set_x(1);
  p.set_y(3);

  q.set_x(1);
  q.set_y(6);

  r.set_x(-1);
  r.set_y(-1);

  std::cout << "Point 1 at: (" << p.get_x() << ", " << p.get_y() << ")\n"
            << "Point 2 at: (" << q.get_x() << ", " << q.get_y() << ")\n"
            << "Point 3 at: (" << r.get_x() << ", " << r.get_y() << ")\n" << std::endl;

  std::cout << "Distance between P1 and P2: " << p.Distance(p, q) << "\n"
            << "Distance between P2 and P3: " << q.Distance(q, r) << "\n"
            << "Distance between P1 and P3: " << r.Distance(r, p) << "\n" << std::endl;
}
