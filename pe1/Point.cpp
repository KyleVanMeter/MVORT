#include "Point.h"
#include <math.h>
#include <algorithm>

int Point::get_x() {
  return(_x);
}

int Point::get_y() {
  return(_y);
}

void Point::set_x(int x) {
    _x = x;
}

void Point::set_y(int y) {
    _y = y;
}

double Point::Distance(Point a, Point b) {
    int v_dist, h_dist;

    auto value = std::minmax({a._y, b._y});
    v_dist = value.second - value.first;

    value = std::minmax({a._x, b._x});
    h_dist = value.second - value.first;

    return(sqrt((h_dist * h_dist) + (v_dist * v_dist)));
}
