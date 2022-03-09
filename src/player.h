#ifndef player_h
#define player_h

#include "entity.h"

class Player : public Entity {
private:
  int level;
  int exp;
  void levelUp();
  int expToNextLevel();
  bool isLevelUp();

public:
  Player(Point _position, int _health, int _attack);
  Player();
  int getLevel() const;
  int getExp() const;
  void setExp(int _exp);
  // void exploreTreasure(const Treasure &treasure);
};

#endif
