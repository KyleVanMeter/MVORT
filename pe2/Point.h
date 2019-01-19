/*
 * Kyle Van Meter
 * pe1
 * These files implement the Point class, and test them
 */

class Point {
public:
  int get_x();
  int get_y();
  void set_x(int x);
  void set_y(int y);
  double Distance(Point a, Point b);
private:
  int _x;
  int _y;
};
