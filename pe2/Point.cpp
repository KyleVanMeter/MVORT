/*
 * Kyle Van Meter
 * pe1
 * These files implement the Point class, and test them
 */

#include "Point.h"
#include <math.h> //sqrt
#include <algorithm> //minmax
#include <vector>
#include <stdexcept> //invalid_argument

std::vector<int> Point::get_val() {
  return(_val);
}

int Point::get_nth_val(int dim) {
  if(dim > (int)_val.size()) {
    throw std::invalid_argument("In get_val(int dim): dimension outside of range.");
  }

  return(_val[dim]);
}

void Point::set_val(std::vector<int> a) {
  _val = a;
}

void Point::update_val(int dim, int val) {
  if(dim > (int)_val.size()) {
    throw std::invalid_argument("In update_val(int dim, int val): dimension outside of range.");
  }

  _val[dim] = val;
}

/*
 * Calculates the distance between two x,y pairs of the Point class
 * @param two instantitations of the Point class
 * @return the euclidean distance between the two pairs of points
 */
double Point::Distance(Point a, Point b) {
  if(a._val.size() != b._val.size()) {
    throw std::invalid_argument("In Distance(Point a, Point b): dimensions of arguments do not match.");
  }

  int temp = 0;
  int result = 0;
  //for n dimensional euclidean distance c = sqrt((p_1 - q_1)^2 + (p_2 - q_2)^2 + ... (p_n - q_n)^2)
  for(int i = 0; i <= b._val.size(); i++) {
    temp = a._val[i] - b._val[i];
    temp *= temp;
    result += temp;
  }

  return(sqrt(result));
}
