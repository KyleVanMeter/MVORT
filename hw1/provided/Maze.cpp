#include "Maze.h"
#include <assert.h>
#include <iostream>

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
  std::vector<int> a;

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

  /*
   * This is how we call our singleton Generator class.  Get a random number
   * int from the specified range so we can get randomized treasure icons
   */
  result = lootVec.at(Generator::GetInstance().GetRandomInt(0, lootVec.size()-1));

  /*
   * Order matters here!
   */
  std::vector<std::string> symbolVec =
  { "█",
    "╳",
    ".",
    "◯",
    "▣",
    result
  };

  /*
   * The idea here is that we can iterate over the enum, and associate each
   * items numerical value with the correct element in the vector symbolVec
   */
  for(SquareType iter = SquareType::Wall; iter != SquareType::MAX;
       iter = static_cast<SquareType>(static_cast<int>(iter) + 1)) {
    if(sq == iter) {
      return(symbolVec.at(static_cast<int>(iter)));
    }
  }

  return("?");
}

Board::Board() {
  //TODO: possibly move this down into the Maze constructor as an if/else block
  for(int i = 0; i < BOARDDIM; i++) {
    for(int j = 0; j < BOARDDIM; j++) {
      arr_[i][j] = SquareType::Empty;
    }
  }

  arr_[BOARDDIM-1][BOARDDIM-1] = SquareType::Exit;
  arr_[0][0] = SquareType::Human;
}

SquareType Board::get_square_value(Position pos) const {
  return arr_[pos.row][pos.col];
}

void Board::SetSquareValue(Position pos, SquareType value) {
  arr_[pos.row][pos.col] = value;
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

Maze::Maze() {
  int random = 0;
  board_ = new Board;

  /*
   * We are starting at [0][1] as [0][0] will always be SquareType::Human, also
   * walls are a 20% chance on each square so we just pick a random number
   * between 0, and 4 (1 in 5 chance), and 0 and 9 (1 in 10) for treasure
   */

  for(int i = 0; i < BOARDDIM; i++) {
    for(int j = 1; j < BOARDDIM; j++) {
      random = Generator::GetInstance().GetRandomInt(0, 9);

      if(random == 4) {
        board_->SetSquareValue({i,j}, SquareType::Wall);
      } else {
        board_->SetSquareValue({i,j}, SquareType::Empty);
      }
    }
  }

  for(int i = 0; i < BOARDDIM; i++) {
    for(int j = 1; j < BOARDDIM; j++) {
      random = Generator::GetInstance().GetRandomInt(0, 9);

      if(random == 9 &&
         board_->get_square_value({i, j}) != SquareType::Wall) {
        board_->SetSquareValue({i, j}, SquareType::Treasure);
      }
    }
  }

  board_->SetSquareValue({BOARDDIM-1,BOARDDIM-1}, SquareType::Exit);
}

void Maze::PrintMaze() {
  // for debug
  std::cout << "\n";
  for (int i = 0; i < BOARDDIM; i++) {
    for (int j = 0; j < BOARDDIM; j++) {
      std::cout << SquareTypeStringify(board_->get_square_value({i, j}));
    }

    std::cout << "\n";
  }
}

void Maze::NewGame(Player *human, const int enemies) {
  std::string name = "Monster_";
  turnOrder_.push(human);

  for(int x = 0; x < enemies; x++) {
    int row = 0;
    int col = 0;

    /*
     * This attempts to ensure that we do not place an enemy on a tile
     * that is the player's starting position, a wall, or the exit
     */
    while ((row == 0 && col == 0) ||
           (board_->get_square_value({row, col}) == SquareType::Wall) ||
           (board_->get_square_value({row, col}) == SquareType::Exit)) {

      row = Generator::GetInstance().GetRandomInt(0, BOARDDIM-1);
      col = Generator::GetInstance().GetRandomInt(0, BOARDDIM-1);
    }

    name += std::to_string(x);
    Player * mon = new Player(name, false);
    mon->SetPosition({row,col});

    board_->SetSquareValue({row, col}, SquareType::Enemy);

    turnOrder_.push(mon);
  }
}
