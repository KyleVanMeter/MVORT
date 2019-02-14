#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Rectangle.h"


TEST_CASE ( "Factorials are computed", "[factorial]") {
  REQUIRE( Factorial(2) == 2 );
  REQUIRE( Factorial(3) == 6 );
}

TEST_CASE("Width is computed", "[GetWidth]") {
  Rectangle a({0, 0}, {1, 1});
  REQUIRE(a.GetWidth() == 1);

  Rectangle b({-2, -2}, {-1, -1});
  REQUIRE(b.GetWidth() == 1);

  Rectangle c({-1, -1}, {1, 1});
  REQUIRE(c.GetWidth() == 2);

  Rectangle d({1, 1}, {1, 2});
  REQUIRE(d.GetWidth() == 0);
}

TEST_CASE("Height is computed.", "[GetHeight]") {
  Rectangle a({0, 0}, {1, 1});
  REQUIRE(a.GetHeight() == 1);

  Rectangle b({-2, -2}, {-1, -1});
  REQUIRE(a.GetHeight() == 1);

  Rectangle c({-1, -1}, {1, 1});
  REQUIRE(a.GetHeight() == 2);

  Rectangle d({1, 1}, {1, 2});
  REQUIRE(a.GetHeight() == 0);
}

TEST_CASE("get_p1 is computed.", "[get_p1]") {
  Rectangle a({0, 0}, {1, 1});
  Point ret1 = {0, 0};
  bool one = (a.get_p1().x == ret1.x) && (a.get_p1().y == ret1.y);

  REQUIRE(one);

  Rectangle b({-1, -2}, {1, 1});
  Point ret2 = {-1, -2};
  bool two = (b.get_p1().x == ret2.x) && (b.get_p1().y == ret2.y);

  REQUIRE(two);

  Rectangle c({1, 2}, {1, 1});
  Point ret3 = {1, 2};
  bool three = (c.get_p1().x == ret3.x) && (c.get_p1().y == ret3.y);

  REQUIRE(three);
}

TEST_CASE("get_p2 is computed.", "[get_p2]") {
  Rectangle a({-1, -1}, {0, 0});
  Point ret1 = {0, 0};
  bool one = (a.get_p2().x == ret1.x) && (a.get_p2().y == ret1.y);

  REQUIRE(one);

  Rectangle b({-2, -3}, {-1, -2});
  Point ret2 = {-1, -2};
  bool two = (b.get_p2().x == ret2.x) && (b.get_p2().y == ret2.y);

  REQUIRE(two);

  Rectangle c({0, 0}, {1, 2});
  Point ret3 = {1, 2};
  bool three = (c.get_p2().x == ret3.x) && (c.get_p2().y == ret3.y);

  REQUIRE(three);
}

TEST_CASE("Overlaps is computed.", "[Overlaps]") {
  Rectangle a1({0, 0}, {2, 2});
  Rectangle a2({2, 2}, {3, 3});

  REQUIRE(a1.Overlaps(a2));
  REQUIRE(a2.Overlaps(a1));

  Rectangle a0({-1, 0}, {0, 1});
  REQUIRE(a1.Overlaps(a0));
  REQUIRE(a0.Overlaps(a1));

  Rectangle b1({0, -2}, {2, 0});
  Rectangle b2({2, -2}, {3, 0});

  REQUIRE(b1.Overlaps(b2));
  REQUIRE(b2.Overlaps(b1));

  Rectangle b0({-1, -1}, {0, -1});
  REQUIRE(b1.Overlaps(b0));
  REQUIRE(b0.Overlaps(b1));
}

TEST_CASE("CalculateArea is computed.", "[CalculateArea]") {
  Rectangle a({0, 0}, {1, 1});
  REQUIRE(a.CalculateArea() == 1);

  Rectangle b({-1, 0}, {0, 1});
  REQUIRE(b.CalculateArea() == 1);

  Rectangle c({-1, -1}, {0, 0});
  REQUIRE(c.CalculateArea() == 1);

  Rectangle d({0, -1}, {1, 0});
  REQUIRE(d.CalculateArea() == 1);

  Rectangle e({-1, -1}, {1, 1});
  REQUIRE(e.CalculateArea() == 4);

  Rectangle f({-1, -1}, {0, 1});
  REQUIRE(f.CalculateArea() == 2);

  Rectangle g({-1, 0}, {1, 1});
  REQUIRE(g.CalculateArea() == 2);

  Rectangle h({0, -1}, {1, 1});
  REQUIRE(h.CalculateArea() == 2);

  Rectangle i({-1, -1}, {1, 0});
  REQUIRE(i.CalculateArea() == 2);
}

TEST_CASE("Expand is calculated", "[Expand()]") {
  Rectangle a({0, 0}, {1, 1});
  a.Expand();
  bool IsATrue = (a.get_p1().x == -1 && a.get_p1().y == -1) &&
                 (a.get_p2().x == 2 && a.get_p2().y == 2);
  REQUIRE(IsATrue);

  Rectangle b({-1, 0}, {0, 1});
  b.Expand();
  bool IsBTrue = (b.get_p1().x == -2 && b.get_p1().y == -1) &&
                 (b.get_p2().x == 1 && b.get_p2().y == 2);
  REQUIRE(IsBTrue);

  Rectangle c({-1, -1}, {0, 0});
  c.Expand();
  bool IsCTrue = (c.get_p1().x == -2 && c.get_p1().y == -2) &&
                 (c.get_p2().x == 1 && c.get_p2().y == 1);
  REQUIRE(IsCTrue);

  Rectangle d({0, -1}, {1, 0});
  d.Expand();
  bool IsDTrue = (d.get_p1().x == -1 && d.get_p1().y == -2) &&
                 (d.get_p2().x == 2 && d.get_p2().y == 1);
  REQUIRE(IsDTrue);

  Rectangle e({-1, -1}, {1, 1});
  e.Expand();
  bool IsETrue = (e.get_p1().x == -2 && e.get_p1().y == -2) &&
                 (e.get_p2().x == 2 && e.get_p2().y == 2);
  REQUIRE(IsETrue);
}

TEST_CASE("Shrink is calculated", "[Shrink()]") {
  Rectangle a({-1, -1}, {2, 2});
  a.Shrink();
  bool IsATrue = (a.get_p1().x == 0 && a.get_p1().y == 0) &&
                 (a.get_p2().x == 1 && a.get_p2().y == 1);
  REQUIRE(IsATrue);

  Rectangle b({-2, 1}, {1, 2});
  b.Shrink();
  bool IsBTrue = (b.get_p1().x == -1 && b.get_p1().y == 0) &&
                 (b.get_p2().x == 0 && b.get_p2().y == 1);
  REQUIRE(IsBTrue);

  Rectangle c({-2, -2}, {1, 1});
  c.Shrink();
  bool IsCTrue = (c.get_p1().x == -1 && c.get_p1().y == -1) &&
                 (c.get_p2().x == 0 && c.get_p2().y == 0);
  REQUIRE(IsCTrue);

  Rectangle d({-1, -2}, {2, 1});
  d.Shrink();
  bool IsDTrue = (d.get_p1().x == 0 && d.get_p1().y == -1) &&
                 (d.get_p2().x == 1 && d.get_p2().y == 0);
  REQUIRE(IsDTrue);

  Rectangle e({-2, -2}, {2, 2});
  e.Shrink();
  bool IsETrue = (e.get_p1().x == -1 && e.get_p1().y == -1) &&
                 (e.get_p2().x == 1 && e.get_p2().y == 1);
  REQUIRE(IsETrue);
}
