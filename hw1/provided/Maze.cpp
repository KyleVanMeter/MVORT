#include "Maze.h"
#include <assert.h>

Generator::Generator() {
  gen_ = SetGenerator();
}

std::mt19937 Generator::SetGenerator() {
  /*
   * Gets random number from hardware to seed the Mersenne Twister random number
   * generator to get random numbers uniformly.
   */
  std::random_device rnd;
  std::mt19937 eng(rnd());

  return eng;
}

int Generator::GetRandomInt(int min, int max) {
  assert(min < max);
  std::uniform_int_distribution<> dist(min, max);
  distr_ = dist;

  return(distr_(gen_));
}


std::string SquareTypeStringify(SquareType sq) {
  std::string result;

  std::vector<std::string> lootVec = {"▭", "▮", "▯", "▰", "▱", "▲", "△", "▴",
                                      "▵", "▶", "▷", "▸", "▹", "►", "▻", "▼",
                                      "▽", "▾", "▿", "◀", "◁", "◂", "◃", "◄",
                                      "◅", "◆", "◇", "◈", "◉", "◊", "○", "◌",
                                      "◍", "◎", "●", "◐", "◑", "◒", "◓", "◔",
                                      "◕", "◖", "◗"};


  if(sq == SquareType::Wall) {
    result = "█";

    return result;
  }

  if(sq == SquareType::Exit) {
    result = "╳";

    return result;
  }

  if(sq == SquareType::Empty) {
    result = ":";

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
    /*
     * This is how we call our singleton Generator class.  Get a random number
     * int from the specified range so we can get randomized treasure icons
     */
    result = lootVec[Generator::GetInstance().GetRandomInt(0, lootVec.size())];

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

  Position copy = p->get_position();

  bool isUValid = !(get_square_value({copy.row--, copy.col}) == SquareType::Wall
                   || copy.row-- < 0);
  bool isDValid = !(get_square_value({copy.row++, copy.col}) == SquareType::Wall
                   || copy.row++ > BOARDDIM);
  bool isRValid = !(get_square_value({copy.row, copy.col++}) == SquareType::Wall
                   || copy.col++ > BOARDDIM);
  bool isLValid = !(get_square_value({copy.row, copy.col--}) == SquareType::Wall
                    || copy.col-- < 0);

  if(isUValid) {
    moves.push_back({copy.row--, copy.col});
  }

  if(isDValid) {
    moves.push_back({copy.row++, copy.col});
  }

  if(isRValid) {
    moves.push_back({copy.row, copy.col++});
  }

  if(isLValid) {
    moves.push_back({copy.row, copy.col--});
  }

  return moves;
}

bool Board::MovePlayer(Player *p, Position pos) {
  /*
   * Here we are checking if the position to move to is in the list of legal
   * moves.
   */
  for(auto i : GetMoves(p)) {
    if(pos == i) {
      p->SetPosition(pos);

      return true;
    }
  }

  return false;
}

SquareType Board::GetExitOccupant() {
  return SquareType::Exit;
}

void Maze::NewGame(Player *human, const int enemies) {
  
}
