#ifndef PLAYER_H
#define PLAYER_H

#include "movable_entity.h"

class Player : public MovableEntity {
private:
  bool isLevelUp() const;

public:
  Player();
  ~Player() override;

  void addExperience(int _exp);
  void heal(int _health);
  void increaseStrength(int _strength);

  void levelUp();
  int getMaxHealth() const;
  int expToNextLevel() const;

  std::string toString() const override;

  // data
  int level;
  int exp;
};

#endif // PLAYER_H
