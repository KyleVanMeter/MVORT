#include "Maze.h"
#include <random>

std::string SquareTypeStringify(SquareType sq) {
  std::string result;

  std::vector<std::string> lootVec = {"▭", "▮", "▯", "▰", "▱", "▲", "△", "▴",
                                      "▵", "▶", "▷", "▸", "▹", "►", "▻", "▼",
                                      "▽", "▾", "▿", "◀", "◁", "◂", "◃", "◄",
                                      "◅", "◆", "◇", "◈", "◉", "◊", "○", "◌",
                                      "◍", "◎", "●", "◐", "◑", "◒", "◓", "◔",
                                      "◕", "◖", "◗"};

  /*
   * used for getting random treasure icon.  Gets random number from hardware to
   * seed the Mersenne Twister random number generator to get random numbers
   * uniformly
   *
   * TODO: The generator itself should probably exist in some different space so
   * we don't unnecessarily recreate the objects every time the function is
   * called
   */
  std::random_device rnd;
  std::mt19937 eng(rnd());
  std::uniform_int_distribution<> distro(0, lootVec.size());

  if(sq == SquareType::Wall) {
    result = "█";

    return result;
  }

  if(sq == SquareType::Exit) {
    result = "╳";

    return result;
  }

  if(sq == SquareType::Empty) {
    result = " ";

    return result;
  }

  if(sq == SquareType::Human) {
    result = "◯";

    return result;
  }

  if(sq == SquareType::Enemy) {
    result = "▣";

    return result;

  }

  if(sq == SquareType::Treasure) {
    result = lootVec[distro(eng)];

    return result;
  }

  result = "?";
  return result;
}

SquareType Board::get_square_value(Position pos) const {
  return arr_[pos.row][pos.col];
}

std::vector<Position> Board::GetMoves(Player *p) {
  std::vector<Position> moves;
  if(p.get_position().row 
}
