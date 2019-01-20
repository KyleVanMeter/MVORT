/*
 * Kyle Van Meter
 * pe1
 * These files implement the Point class, and test them
 */

#include <iostream>
#include <algorithm>
#include <iterator> //for ostream
#include "Point.h"

int main(int argc, char* argv[]) {
  Point p, q, r;

  std::vector<int> a = {0,0,0};
  p.set_val(a);

  a = {1,1,1};
  q.set_val(a);

  a = {2,3,4};
  r.set_val(a);

  std::cout << "P1 values: { ";
  for(auto const& x : p.get_val()) {
    std::cout << x << " ";
  }
  std::cout << "}\n";

  std::cout << "P2 values: { ";
  for(auto const& x : q.get_val()) {
    std::cout << x << " ";
  }
  std::cout << "}\n";

  std::cout << "P3 values: { ";
  for(auto const& x : r.get_val()) {
    std::cout << x << " ";
  }
  std::cout << "}\n";

  std::cout << "Distance between P1, P2: " << p.Distance(p, q) << "\n"
            << "Distance between P2, P3: " << q.Distance(q, r) << "\n"
            << "Distance between P1, P3: " << r.Distance(p, r) << std::endl;
}
