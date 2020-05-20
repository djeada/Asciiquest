#ifndef monster_h
#define monster_h

#include "game_settings.h"
#include "map.h"
#include "utils.h"
#include <ncurses.h>

class Monster {
private:
  int x;
  int y;
  int vx;
  int vy;
  int attack;
  int health;
  char tile;

public:
  Monster(int _x, int _y, char t);
  void draw();
  void moveMonster(Map myMap, int playerX, int playerY);
  bool checkNoColisions(Map myMap, int playerX, int playerY);

  int getX();
  int getY();
  int getAttack();
  int getHealth();
  char getTile();

  void randomizeVelocity();
  void setHealth(int a);
};

std::vector<Monster> initalizeMonsters(int how_many, Map myMap);

#endif
