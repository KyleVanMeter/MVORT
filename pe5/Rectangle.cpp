#include "Rectangle.h"
#include <cmath>

Rectangle::Rectangle(Point p1, Point p2) {
  p1_ = p1;
  p2_ = p2;
}

int Rectangle::GetWidth() { return (sqrt(pow((p2_.x - p1_.x), 2.0))); }

int Rectangle::GetHeight() { return (sqrt(pow((p2_.y - p1_.y), 2.0))); }

bool Rectangle::Overlaps(Rectangle &other) {
  Point otherP1 = other.get_p1();
  Point otherP2 = other.get_p2();

  if (otherP1.x <= p2_.x &&
      otherP2.x >= p1_.x &&
      otherP2.y >= p1_.y &&
      otherP1.y <= p2_.y) {
    return true;
  }
  return false;
}

int Rectangle::CalculateArea() {
  int x = sqrt( pow(p2_.x - p1_.x, 2));
  int y = sqrt( pow(p2_.y - p1_.y, 2));

  return(x * y);
}

void Rectangle::Expand() {
  p1_.x -= 1;
  p1_.y -= 1;

  p2_.x += 1;
  p2_.y += 1;
}

void Rectangle::Shrink() {
  p1_.x += 1;
  p1_.y += 1;

  p2_.x -= 1;
  p2_.y -= 1;
}
