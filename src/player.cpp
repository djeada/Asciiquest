#include "player.h"
#include "game_settings.h"
#include <cmath>
#include <ncurses.h>
#include <stdio.h>

Player::Player(const Point &_position, int _health, int _attack)
    : Entity(_position, _health, _attack, Represetiation('@', PLAYER_COLOR)) {
  level = 1;
  exp = 0;
}

Player::Player() : Entity(Point(), 100, 10, Represetiation('@', PLAYER_COLOR)) {
  level = 1;
  exp = 0;
}

void Player::levelUp() {
  if (!isLevelUp())
    return;

  level++;
  exp -= expToNextLevel();
}

auto Player::expToNextLevel() const -> int {
  return level * 100 * pow(1.25, level - 1);
}

auto Player::isLevelUp() -> bool { return exp >= expToNextLevel(); }

void Player::setExp(int _exp) {
  exp = _exp;
  if (isLevelUp())
    levelUp();
}

auto Player::getLevel() const -> int { return level; }
auto Player::getExp() const -> int { return exp; }
