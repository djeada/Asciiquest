#ifndef MONSTER_H
#define MONSTER_H

#include "model/map.h"
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

  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;
  int followRange;

public:
  explicit Goblin(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player);
  void move(const Point &destination) override;
  void randomizeVelocity() override;
  std::string toString() const override;
};

class Orc : public Monster {

  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;
  std::deque<Point> path;
  std::mutex mutex;
  int followRange;

public:
  explicit Orc(std::shared_ptr<Map> _map, std::shared_ptr<Player> player);
  void move(const Point &destination) override;
  std::string toString() const override;
  void randomizeVelocity() override;
  Point getVelocity() override;
};

class Troll : public Monster {

  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;
  int followRange;

public:
  explicit Troll(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player);
  void move(const Point &destination) override;
  void randomizeVelocity() override;
  std::string toString() const override;
};

class Dragon : public Monster {

public:
  explicit Dragon();
  void move(const Point &destination) override;
  void randomizeVelocity() override;
  std::string toString() const override;
};

class Skeleton : public Monster {

  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;
  int followRange;

public:
  explicit Skeleton(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player);
  void move(const Point &destination) override;
  void randomizeVelocity() override;
  std::string toString() const override;
};

#endif // MONSTER_H
