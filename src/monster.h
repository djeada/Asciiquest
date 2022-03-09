#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MONSTER_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MONSTER_H

#include "entity.h"
#include "game_settings.h"
#include "map.h"
#include "utils.h"
#include <ncurses.h>

class Monster : public Entity {
public:
  Monster(const Point &_position, int _health, int _attack,
          Represetiation _representation);
  void randomizeVelocity();
};

class Goblin : public Monster {
public:
  explicit Goblin(const Point &_position);
  void move(int dx, int dy) override;
};

class Orc : public Monster {
public:
  explicit Orc(const Point &_position);
  void move(int dx, int dy) override;
};

class Troll : public Monster {
public:
  explicit Troll(const Point &_position);
  void move(int dx, int dy) override;
};

class Dragon : public Monster {
public:
  explicit Dragon(const Point &_position);
  void move(int dx, int dy) override;
};

#endif
