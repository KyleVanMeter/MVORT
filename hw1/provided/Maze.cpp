#include "Maze.h"
#include <assert.h>
#include <algorithm>
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
    " ",
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
  //std::cout << "\n p at: {" << copy.row << ", " << copy.col << "}\n";

  bool isUValid = !(get_square_value({copy.row-1, copy.col}) == SquareType::Wall
                   || (copy.row-1) < 0);
  bool isDValid = !(get_square_value({copy.row+1, copy.col}) == SquareType::Wall
                   || (copy.row+1 > BOARDDIM));
  bool isRValid = !(get_square_value({copy.row, copy.col+1}) == SquareType::Wall
                   || (copy.col+1 > BOARDDIM));
  bool isLValid = !(get_square_value({copy.row, copy.col-1}) == SquareType::Wall
                   || (copy.col-1 < 0));

  //std::cout << "copy is now: " << copy.row << ", " << copy.col << "\n";

  if(isUValid) {
    //std::cout << "UP is valid. Pushing: " << copy.row-1 << ", " << copy.col
    //         << "\n";
    moves.push_back({copy.row-1, copy.col});
  }

  if(isDValid) {
    //std::cout << "DOWN is valid. Pushing: " << copy.row+1 << ", " << copy.col
    //          << "\n";
    moves.push_back({copy.row+1, copy.col});
  }

  if(isRValid) {
    //std::cout << "RIGHT is valid.  Pushing: " << copy.row << ", " << copy.col+1
    //          << "\n";
    moves.push_back({copy.row, copy.col+1});
  }

  if(isLValid) {
    //std::cout << "LEFT is valid. Pushing: " << copy.row << ", " << copy.col-1
    //          << "\n";
    moves.push_back({copy.row, copy.col-1});
  }

  //std::cout << "returning: {";
  //for(auto i : moves) {
  //  std::cout << " [" << i.row << ", " << i.col << "],";
  //}
  //
  //std::cout << "}";

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
  turn_count_ = 0;
  board_ = new Board;

  /*
   * We are starting at [0][1] as [0][0] will always be SquareType::Human, also
   * walls are a 20% chance on each square so we just pick a random number
   * between 0 and 4 (1 in 5 chance), 0 and 9 (1 in 10) for treasure, and 0 and
   * 19 (1 in 20 chance) for a pit hazard
   */
  for(int i = 0; i < BOARDDIM; i++) {
    for(int j = 1; j < BOARDDIM; j++) {
      random = Generator::GetInstance().GetRandomInt(0, 4);

      if (random == 4) {
        board_->SetSquareValue({i,j}, SquareType::Wall);
        random = Generator::GetInstance().GetRandomInt(0, 10);
      } else if (random == 2) {
        board_->SetSquareValue({i,j}, SquareType::Pit);
      } else {
        board_->SetSquareValue({i,j}, SquareType::Empty);
      }
    }
  }

  /*
   * If the exit is entirely blocked, we must ensure that there is a valid path
   * This is not any fancy path-finding algorithm, but will deal the most
   * probable event (that the immediate up, and left spaces are walls/pits)
   */
  if ((board_->get_square_value({BOARDDIM - 2, BOARDDIM - 1}) ==
       SquareType::Wall &&
       board_->get_square_value({BOARDDIM - 1, BOARDDIM - 2}) ==
       SquareType::Wall) ||
      (board_->get_square_value({BOARDDIM - 2, BOARDDIM - 1}) ==
       SquareType::Pit &&
       board_->get_square_value({BOARDDIM - 1, BOARDDIM - 2}) ==
       SquareType::Pit)) {

    /*
     * If this is the case then we clear a route along the edge
     */
    for (int i = 0; i < BOARDDIM - 1; i++) {
      board_->SetSquareValue({BOARDDIM - 1, i}, SquareType::Empty);
    }

    for (int j = 1; j < BOARDDIM - 1; j++) {
      board_->SetSquareValue({j, 0}, SquareType::Empty);
    }
  }

  /*
   * We are re-iterating over the board to generate treasure in the hopes that
   * this will help avoid any correlation between walls, and treasure
   */
  for (int i = 0; i < BOARDDIM; i++) {
    for (int j = 1; j < BOARDDIM; j++) {
      random = Generator::GetInstance().GetRandomInt(0, 9);

      if (random == 9 &&
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
  players_.push_back(human);

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
    players_.push_back(mon);
  }
}

void Maze::TakeTurn() {
  Player * Current = turnOrder_.front();
  std::vector<Position> result;

  //move the player taking their turn to the back of the turn queue
  turnOrder_.pop();
  turnOrder_.push(Current);

  if(Current->is_human()) {
    //player's turn
    bool continueLooping = true;
    std::string input;
    std::vector<std::string> validMoves;
    result = board_->GetMoves(Current);

    std::cout << "Valid Moves are: ( ";
    for(Position i : result) {
      validMoves.push_back(Current->ToRelativePosition(i));

      std::cout << Current->ToRelativePosition(i) << " ";
    }
    std::cout << ")\n";

    /*
     * If the input from the user (case insensitive) matches a move in the
     * vector of strings validMoves, then stop looping, and move the player to
     * that position
     */
    while(continueLooping) {
      std::cout << "Enter valid direction: ";
      std::cin >> input;

      //dumb c++ way of upper-casing a string
      std::transform(input.begin(), input.end(), input.begin(), ::toupper);

      for(std::string j : validMoves) {
        if(input == j) {
          continueLooping = false;
        }
      }
    }
    std::cout << "\nTurn #" << turn_count_ << "\n";

    Position CurPosition = {
      Current->get_position().row,
      Current->get_position().col
    };

    /*
     * This block of solid text (garbage) simply moves the player in the
     * direction given via their input, ensures that the board is draw
     * correctly, and deals with dying, picking up treasure, or falling into a
     * pit.  There must be a better way of doing this...
     */
    if (input == "UP") {
      Current->SetPosition({CurPosition.row - 1, CurPosition.col});

      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Treasure) {
        Current->ChangePoints(Current->get_points() + 100);
      }
      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Pit) {
        std::cout << "\nYou fell!  Start over.\n";
        Current->ChangePoints(Current->get_points() - 50);
        Current->SetPosition({0, 0});
        board_->SetSquareValue({0,0}, SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      } else {
        board_->SetSquareValue({CurPosition.row - 1, CurPosition.col},
                               SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      }
    }
    if (input == "DOWN") {
      Current->SetPosition({CurPosition.row + 1, CurPosition.col});

      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Treasure) {
        Current->ChangePoints(Current->get_points() + 100);
      }
      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Pit) {
        std::cout << "\nYou fell!  Start over.\n";
        Current->ChangePoints(Current->get_points() - 50);
        Current->SetPosition({0, 0});
        board_->SetSquareValue({0, 0}, SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      } else {
        board_->SetSquareValue({CurPosition.row + 1, CurPosition.col},
                               SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      }
    }
    if (input == "LEFT") {
      Current->SetPosition({CurPosition.row, CurPosition.col - 1});

      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Treasure) {
        Current->ChangePoints(Current->get_points() + 100);
      }
      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Pit) {
        std::cout << "\nYou fell!  Start over.\n";
        Current->ChangePoints(Current->get_points() - 50);
        Current->SetPosition({0, 0});
        board_->SetSquareValue({0, 0}, SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      } else {
        board_->SetSquareValue({CurPosition.row, CurPosition.col - 1},
                               SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      }
    }
    if (input == "RIGHT") {
      Current->SetPosition({CurPosition.row, CurPosition.col + 1});

      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Treasure) {
        Current->ChangePoints(Current->get_points() + 100);
      }
      if (board_->get_square_value(Current->get_position()) ==
          SquareType::Pit) {
        std::cout << "\nYou fell!  Start over.\n";
        Current->ChangePoints(Current->get_points() - 50);
        Current->SetPosition({0, 0});
        board_->SetSquareValue({0, 0}, SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      } else {
        board_->SetSquareValue({CurPosition.row, CurPosition.col + 1},
                               SquareType::Human);
        board_->SetSquareValue(CurPosition, SquareType::Empty);
      }
    }

    std::cout << "Current Pos: {" << Current->get_position().row
              << ", " << Current->get_position().col << "}\n";


    turn_count_ += 1;
  } else {
    //non-humans turn
    int random = 0;
    result = board_->GetMoves(Current);
    /*
     * we must ensure that min < max when calling GetRandomInt so we must deal
     * the case where there are no valid moves (move to the same space), or
     * only one valid move (do that move)
    */
    if (result.size() > 1) {
      random = Generator::GetInstance().GetRandomInt(0, result.size() - 1);
    }

    /*
     * The enemies sometime make seemingly impossible moves.  I swear its not
     * a bug, but a feature.  Spooky!  Also enemies eat treasure, and will
     * occasionally fill in pits (how kind), which is intended behavior
     */
    board_->SetSquareValue(turnOrder_.back()->get_position(), SquareType::Empty);
    turnOrder_.back()->SetPosition(result.at(random));
    board_->SetSquareValue(result.at(random), SquareType::Enemy);
  }
}

bool Maze::IsGameOver() {
  /*
   * The game is over if the player's position is the same as the exit position,
   * or the player and an enemy share the same position
   */
  Position end = {BOARDDIM-1, BOARDDIM-1};
  bool isDead = false;

  /*
   * This makes the assumption that there is only one player, and in that case
   * it will always be at players_.at(0), so compare its position with the rest
   */
  for (unsigned int i = 1; i < players_.size(); i++) {
    if (players_.at(0)->get_position() == players_.at(i)->get_position()) {
      std::cout << "\nYou died!\n";
      players_.at(0)->ChangePoints(players_.at(0)->get_points() - 100);
      isDead = true;
    }
  }

  return ((turnOrder_.back()->is_human() &&
           turnOrder_.back()->get_position() == end) ||
          isDead);
}
