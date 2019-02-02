#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

struct Position {
	int row;
	int col;

	// already implemented for you!
	bool operator==(const Position &other) {
    return row == other.row && col == other.col;
	}
};


class Player {
public:
	Player(const std::string name, const bool is_human);  // constructor

	// These are already implemented for you
	std::string get_name() const {return name_; }  // inline member function
	int get_points() const {return points_; }  // inline member function
	Position get_position() const {return pos_; }  // inline member function
	bool is_human() const {return is_human_; }  // inline member function

  void ChangePoints(const int x);

  void SetPosition(Position pos);

  // ToRelativePosition is a function we used to translate positions
  // into directions relative to the player (up, down, etc)
  std::string ToRelativePosition(Position other);

  // Convert this player to a string representation of their name and points
  std::string Stringify();

private:
	std::string name_;
	int points_;
	Position pos_;
	bool is_human_;
};

#endif  // _PLAYER_H_
