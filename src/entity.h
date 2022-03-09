#ifndef entity_h
#define entity_h

#include "utils.h"

struct Represetiation {
  char symbol;
  int color;
  Represetiation(char _symbol, int _color) : symbol(_symbol), color(_color) {}
};

class Entity {
protected:
  Point position;
  Point velocity;
  int health;
  int strength;
  Represetiation representation;

public:
  Entity(Point _position, int _health, int _attack,
         Represetiation _representation);
  Entity();
  virtual ~Entity();

  Point getPosition();
  int getHealth() const;
  int getAttack() const;
  char getSymbol() const;
  bool isAlive();
  void setPosition(Point _position);
  void setHealth(int _health);
  void setAttack(int _attack);
  void draw();
  virtual void move(int dx = 1, int dy = 1);
  virtual void takeDamage(int damage);

  friend void attack(Entity &attacker, Entity &defender);
};

#endif
