#ifndef _MONSTER_H
#define _MONSTER_H

#include "map.h"
#include "movable_entity.h"
#include "player.h"
#include <deque>
#include <memory>
#include <mutex>
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

  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;
  std::deque<Point> path;
  std::mutex mutex;

public:
  explicit Orc(std::shared_ptr<Map> _map, std::shared_ptr<Player> player);
  void move(const Point &destination);
  auto toString() const -> std::string override;
  void randomizeVelocity() override;
  Point getVelocity() override;
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
