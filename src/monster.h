#ifndef monster_h
#define monster_h

#include "entity.h"
#include "game_settings.h"
#include "map.h"
#include "utils.h"
#include <ncurses.h>

class Monster : public Entity {
public:
  Monster(Point _position, int _health, int _attack,
          Represetiation _representation);
  void randomizeVelocity();
};

class Goblin : public Monster {
public:
  Goblin(Point _position);
  void move(int dx, int dy);
};

class Orc : public Monster {
public:
  Orc(Point _position);
  void move(int dx, int dy);
};

class Troll : public Monster {
public:
  Troll(Point _position);
  void move(int dx, int dy);
};

class Dragon : public Monster {
public:
  Dragon(Point _position);
  void move(int dx, int dy);
};

#endif
