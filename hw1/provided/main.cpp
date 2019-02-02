#include "Maze.h"
#include <iostream>

int main() {
  Player p1 = Player("test", true);
  p1.ChangePoints(10);
  std::cout << "Testing Player object\np1.get_name() = " + p1.get_name() + "\np1.get_points = " + std::to_string(p1.get_points()) + "\nDefault constructor position: " + "{" + std::to_string(p1.get_position().row) + ", " + std::to_string(p1.get_position().col) + "}\n" + "p1.is_human() = " + std::to_string(p1.is_human()) + "\n";

  p1.SetPosition({1,1});

  std::cout << "Updating Position\np1.get_position() = {" + std::to_string(p1.get_position().row) + ", " + std::to_string(p1.get_position().col) + "}\nTesting relative positions\n{0,1} = " + p1.ToRelativePosition({0,1}) + "\n{2,1} = " + p1.ToRelativePosition({2,1}) + "\n{1,2} = " + p1.ToRelativePosition({1,2}) + "\n{1,0} = " + p1.ToRelativePosition({1,0}) + "\n{-1,-1} = " + p1.ToRelativePosition({-1,-1}) + "\n\n";

  return 0;
}
