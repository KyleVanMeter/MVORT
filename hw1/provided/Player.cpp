#include "Player.h"
#include <iostream>

Player::Player(std::string name, const bool is_human) {
  name_ = name;
  points_ = 0;
  pos_ = {0,0};
  is_human_ = is_human;
}

void Player::ChangePoints(const int x) {
  points_ = x;
}

void Player::SetPosition(Position pos) {
  pos_.row = pos.row;
  pos_.col = pos.col;
}


std::string Player::ToRelativePosition(Position other) {
  //this is assuming that Position other is restricted to adjacent points

  std::cout << "internal: {" + std::to_string(pos_.row) + ", " + std::to_string(pos_.col) + "}, and {" + std::to_string(other.row) + ", " + std::to_string(other.col) + "}\n";

  std::string result;
  if(pos_.row == other.row && pos_.col-1 == other.col) {
    result = "LEFT";
    return result;
  }

  if(pos_.row == other.row && pos_.col+1 == other.col) {
    result = "RIGHT";
    return result;
  }

  if(pos_.row+1 == other.row && pos_.col == other.col) {
    result = "DOWN";
    return result;
  }

  if(pos_.row-1 == other.row && pos_.col == other.col) {
    result = "UP";
    return result;
  }

  result = "";
  return result;
}

std::string Player::Stringify() {
  return(name_ + ":" + std::to_string(points_));
}
