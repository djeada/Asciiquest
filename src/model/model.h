#ifndef MODEL_H
#define MODEL_H

#include "map.h"
#include "monster.h"
#include "player.h"
#include "utils/direction.h"
#include <atomic>
#include <memory> // for std::shared_ptr
#include <mutex>
#include <unordered_map>
#include <vector>
class Model {

public:
  Model();
  void update();
  void fight(Monster &monster);
  void movePlayer(Point point);
  void loadMap();
  bool isGameOver();
  std::unordered_map<std::string, std::string> getPlayerStats();
  void stopUpdateLoop();
  std::vector<std::string> info;
  std::shared_ptr<Map> map;
  Player player;
  std::vector<std::shared_ptr<Monster>> monsters;

private:
  void moveEntity(Point point, Entity &entity);
  void updateEntityPosition(Point oldPos, Point newPos);
  bool isWall(Point point);
  bool isPlayer(Point point);
  bool isMonster(Point point);
  void startUpdateLoop();
  std::atomic_bool running;
  std::mutex mapMutex;
  std::mutex fightMutex;
};

#endif // MODEL_H
