#ifndef MODEL_H
#define MODEL_H

#include "monster.h"
#include "player.h"
#include "utils/direction.h"
#include <unordered_map>
#include <vector>

class Model {

  // map: wall positions, start and end
  // player, has position, can move, can fight
  // enemies, has position, can move, can fight
  // tresure, has position, can move, can fight

  // handles AI and collision on high level, details implemented in lower level
  // classes

public:
  Model();
  void update();
  void fight(Monster &monster);
  void movePlayer(Point point);
  void loadMap();
  bool isGameOver();
  std::unordered_map<std::string, std::string> getPlayerStats();

  std::vector<std::string> fightInfo;
  std::vector<std::vector<CellType>> grid;
  Player player;
  std::vector<Monster> monsters;

private:
  void updateEntityPosition(Point oldPos, Point newPos);
  bool isWall(Point point);
  bool isPlayer(Point point);
};

#endif // MODEL_H
