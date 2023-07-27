#ifndef PLAYER_H
#define PLAYER_H

#include "movable_entity.h"

class Player : public MovableEntity {
private:
  bool isLevelUp() const;

public:
  Player();
  ~Player();

  void levelUp();
  int expToNextLevel() const;
  void addExperience(int _exp);
  std::string toString() const override;
  // void exploreTreasure(const Treasure &treasure);

  // data
  int level;
  int exp;
};

#endif
