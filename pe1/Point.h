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
