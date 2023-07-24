#ifndef MODEL_H
#define MODEL_H

#include "map.h"
#include "monster.h"
#include "player.h"
#include "utils/direction.h"
#include <memory> // for std::shared_ptr
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

  std::vector<std::string> fightInfo;
  std::shared_ptr<Map> map; // Use std::shared_ptr for the Map object
  Player player;
  std::vector<Monster> monsters;

private:
  void updateEntityPosition(Point oldPos, Point newPos);
  bool isWall(Point point);
  bool isPlayer(Point point);
};

#endif // MODEL_H
