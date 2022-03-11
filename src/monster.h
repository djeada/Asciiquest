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
  auto toString() const -> std::string override;
};

class Goblin : public Monster {
public:
  explicit Goblin(const Point &_position);
  void move(int dx = 1, int dy = 1) override;
  auto toString() const -> std::string override;
};

class Orc : public Monster {
public:
  explicit Orc(const Point &_position);
  void move(int dx = 1, int dy = 1) override;
  auto toString() const -> std::string override;
};

class Troll : public Monster {
public:
  explicit Troll(const Point &_position);
  void move(int dx = 1, int dy = 1) override;
  auto toString() const -> std::string override;
};

class Dragon : public Monster {
public:
  explicit Dragon(const Point &_position);
  void move(int dx, int dy) override;
  auto toString() const -> std::string override;
};

#endif
