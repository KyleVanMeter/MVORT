/*
 * Kyle Van Meter
 * pe2
 * These files implement the Point class, and test them
 */
#include <vector>

class Point {
public:
  std::vector<int> get_val();
  int get_nth_val(int dim);

  void update_val(int dim, int val);
  void set_val(std::vector<int> vec);

  double Distance(Point a, Point b);
private:
  std::vector<int> _val;
  int _dim;
};
