#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
#include <deque>

class Monster : public Entity {

public:
  Monster(const Point &_position, int _health, int _attack);
  void randomizeVelocity();
  auto toString() const -> std::string override;
};

class Goblin : public Monster {

public:
  explicit Goblin(const Point &_position);
  void move(const Point &destination) override;
  void moveBy(const Point &destination) override;
  auto toString() const -> std::string override;
};

class Orc : public Monster {

  std::deque<Point> path;

public:
  explicit Orc(const Point &_position);
  void move(const Point &destination);
  auto toString() const -> std::string override;
  void setPath(const std::deque<Point> &_path);
  auto isPathEmpty() const -> bool;
};

class Troll : public Monster {

public:
  explicit Troll(const Point &_position);
  void move(const Point &destination);
  auto toString() const -> std::string override;
};

class Dragon : public Monster {

public:
  explicit Dragon(const Point &_position);
  void move(const Point &destination);
  auto toString() const -> std::string override;
};

#endif
