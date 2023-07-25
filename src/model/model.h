#ifndef MODEL_H
#define MODEL_H

#include "map.h"
#include "monster.h"
#include "player.h"
#include "utils/direction.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class Model {

public:
  Model();
  void update();
  void fight(Monster &monster);
  void queuePlayerMove(const Point &point);
  void loadMap();
  bool isGameOver();
  std::unordered_map<std::string, std::string> getPlayerStats();

  Player player;
  std::shared_ptr<Map> map;
  std::vector<std::string> info;
  std::vector<std::shared_ptr<Monster>> monsters;

private:
  void attemptPlayerMove(Player &player, const Point &direction);
  void attemptMonsterMove(Monster &monster, const Point &direction);
  void attemptEntityMove(Entity &entity, const Point &point);
  void updateEntityPosition(Entity &entity, const Point &oldPos,
                            const Point &newPos);
  bool isWall(const Point &point);
  bool isPlayer(const Point &point);
  bool isMonster(const Point &point);
  std::atomic_bool running;
};

#endif // MODEL_H
