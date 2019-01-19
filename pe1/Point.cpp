/*
 * Kyle Van Meter
 * pe1
 * These files implement the Point class, and test them
 */

#include "Point.h"
#include <math.h> //sqrt
#include <algorithm> //minmax

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
/*
 * Calculates the distance between two x,y pairs of the Point class
 * @param two instantitations of the Point class
 * @return the euclidean distance between the two pairs of points
 */
double Point::Distance(Point a, Point b) {
    //v_dist, and h_dist are the distances between just the y-components, and just the x-components
    int v_dist, h_dist;

    //value is of type std::pair() s.t value.second >= value.first
    auto value = std::minmax({a._y, b._y});
    v_dist = value.second - value.first;

    value = std::minmax({a._x, b._x});
    h_dist = value.second - value.first;

    //c = sqrt(a^2 + b^2)
    return(sqrt((h_dist * h_dist) + (v_dist * v_dist)));
}
