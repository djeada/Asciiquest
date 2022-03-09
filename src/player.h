#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_PLAYER_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_PLAYER_H

#include "entity.h"

class Player : public Entity {
private:
  int level;
  int exp;
  void levelUp();
  auto expToNextLevel() const -> int;
  auto isLevelUp() -> bool;

public:
  Player(const Point &_position, int _health, int _attack);
  Player();
  auto getLevel() const -> int;
  auto getExp() const -> int;
  void setExp(int _exp);
  // void exploreTreasure(const Treasure &treasure);
};

#endif
