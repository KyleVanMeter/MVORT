#include "Maze.h"
#include <iostream>

int main() {
  Player p1 = Player("test", true);
  /*
  p1.ChangePoints(10);
  std::cout << "Testing Player object\np1.get_name() = " + p1.get_name()
    + "\np1.get_points = " + std::to_string(p1.get_points())
    + "\nDefault constructor position: " + "{" + std::to_string(p1.get_position().row) + ", " + std::to_string(p1.get_position().col) + "}\n"
    + "p1.is_human() = " + std::to_string(p1.is_human()) + "\n";

  p1.SetPosition({1,1});

  std::cout << "Updating Position\np1.get_position() = {" + std::to_string(p1.get_position().row) + ", " + std::to_string(p1.get_position().col)
    + "}\nTesting relative positions\n{0,1} = "
    + p1.ToRelativePosition({0,1}) + "\n{2,1} = "
    + p1.ToRelativePosition({2,1}) + "\n{1,2} = "
    + p1.ToRelativePosition({1,2}) + "\n{1,0} = "
    + p1.ToRelativePosition({1,0}) + "\n{5,5} = "
    + p1.ToRelativePosition({BOARDDIM+1,BOARDDIM+1}) + "\n\n";


  int test0 = Generator::GetInstance().GetRandomInt(0, 10);
  int test1 = Generator::GetInstance().GetRandomInt(0, 10);

  std::cout << "Testing random class singleton: " << test0 <<" " << test1
            << "\nTesting board initialization: \n\n";
  */

  Board b;

  std::cout << "rows: " << b.get_rows() << ", cols: " << b.get_cols() << "\n";

  for(int i = 0; i < BOARDDIM; i++) {
    for(int j = 0; j < BOARDDIM; j++) {
      std::cout << SquareTypeStringify(b.get_square_value({i,j}));
    }
    std::cout << "\n";
  }

  std::cout << "Creating Maze object... ";
  Maze a;

  std::cout << "a.print():\n";
  a.PrintMaze();

  std::cout << "Creating test game with Player p1, and 2 enemies...\n";
  a.NewGame(&p1, 2);

  std::cout << a;

  while(!a.IsGameOver()) {
    a.TakeTurn();
    std::cout << a;

  }

  return 0;
}
