#ifndef _MAZE_H_
#define _MAZE_H_

#define BOARDDIM 4
#include <vector>
#include <random>
#include "Player.h"

enum class SquareType { Wall, Exit, Empty, Human, Enemy, Treasure, MAX };

std::string SquareTypeStringify(SquareType sq);

/*
 * Why is this a singleton?  There are several cases where we need to get a random number across different classes.  Instantiating the random number generator needed, and seeding it is expensive to do repeatedly.  So this is an attempt to create only one generator, and have it be used by all other classes.
 * Of course if we were doing something over a longer period, like simulating Brownian motion, we would have to be careful about correlation from a single generator.  In our case we should be fine.
 */
class Generator {
 public:
  static Generator& GetInstance() {
    static Generator instance;

    return instance;
  }

  int GetRandomInt(int min, int max);
 private:
  std::mt19937 SetGenerator();
  std::mt19937 gen_;
  std::uniform_int_distribution<> distr_;

  Generator(); //The object controls its constructor
};

class Board {
 public:
	Board();

	// already implemented in line
	int get_rows() const {return BOARDDIM; }  // you should be able to change the size of your
	int get_cols() const {return BOARDDIM; }  // board by changing these numbers and the numbers in the arr_ field

  SquareType get_square_value(Position pos) const;

  // set the value of a square to the given SquareType
  void SetSquareValue(Position pos, SquareType value);

  // get the possible Positions that a Player could move to
  // (not off the board or into a wall)
  std::vector<Position> GetMoves(Player *p);

  // Move a player to a new position on the board. Return
  // true if they moved successfully, false otherwise.
  bool MovePlayer(Player *p, Position pos);

  // Get the square type of the exit square
  SquareType GetExitOccupant();

  // You probably want to implement this
  friend std::ostream& operator<<(std::ostream& os, const Board &b);

 private:
	SquareType arr_[BOARDDIM][BOARDDIM];

	int rows_; // might be convenient but not necessary
	int cols_;

	// you may add more fields, as needed
};  // class Board

class Maze {
public:
	// TODO: implement these functions
	Maze(); // constructor

	// initialize a new game, given one human player and
	// a number of enemies to generate
	void NewGame(Player *human, const int enemies);

	// have the given Player take their turn
	void TakeTurn(Player *p);

	// Get the next player in turn order
	Player * GetNextPlayer();

	// return true iff the human made it to the exit
	// or the enemies ate all the humans
	bool IsGameOver();

	// You probably want to implement these functions as well
	// string info about the game's conditions after it is over
	std::string GenerateReport();
	friend std::ostream& operator<<(std::ostream& os, const Maze &m);

private:
	Board *board_;
	std::vector<Player *> players_;
	int turn_count_;

	// you may add more fields, as needed

};  // class Maze

#endif  // _MAZE_H_
