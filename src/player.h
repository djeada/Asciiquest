#ifndef player_h
#define player_h

#include "map.h"
#include "monster.h"
#include "treasure.h"
#include <memory>

class Player {
private:
  int x;
  int y;
  int attack;
  int health;
  int exp;
  int level;
  bool newMapLevelFlag;
  Map myMap;

  void movePlayer(int dx, int dy);
  void increaseExp(char tile);

  bool levelUp();
  bool checkNoColisions(int dx, int dy);

public:
  Player();
  Player(int _x, int _y);

  void draw();
  void handleInput(char ch);
  void fight(std::vector<Monster> &monsters);
  void checkTreasure(std::vector<Treasure> &treasures);
  void turnOffMapLevelFlag();

  int getX();
  int getY();
  int getHealth();
  int getExp();
  int getLevel();
  bool newMapLevel();

  Map getMap();
};

#endif
