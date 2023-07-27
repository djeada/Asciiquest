#ifndef _MONSTER_H
#define _MONSTER_H

#include "movable_entity.h"
#include <deque>
#include <unordered_map>

extern std::unordered_map<CellType, int> monsterExpMap;

class Monster : public MovableEntity {

public:
  Monster(CellType cellType, int _health, int _attack);
  virtual void randomizeVelocity();
};

class Goblin : public Monster {

public:
  explicit Goblin();
  void move(const Point &destination) override;
  auto toString() const -> std::string override;
};

class Orc : public Monster {

  std::deque<Point> path;

public:
  explicit Orc();
  void move(const Point &destination);
  auto toString() const -> std::string override;
  void setPath(const std::deque<Point> &_path);
  auto isPathEmpty() const -> bool;
};

class Troll : public Monster {

public:
  explicit Troll();
  void move(const Point &destination);
  auto toString() const -> std::string override;
};

class Dragon : public Monster {

public:
  explicit Dragon();
  void move(const Point &destination);
  void randomizeVelocity() override;
  auto toString() const -> std::string override;
};

#endif
