#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_ENTITY_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_ENTITY_H

#include "utils.h"
#include <string>

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
  int maxHealth;
  int strength;
  Represetiation representation;

public:
  Entity(const Point &_position, int _health, int _attack,
         Represetiation _representation);
  Entity();
  virtual ~Entity();

  auto getPosition() const -> Point;
  auto getHealth() const -> int;
  auto getMaxHealth() const -> int;
  auto getAttack() const -> int;
  auto getSymbol() const -> char;
  auto isAlive() const -> bool;
  void setPosition(const Point &_position);
  void setHealth(int _health);
  void setAttack(int _attack);
  virtual void draw();
  virtual void move(int dx, int dy);
  virtual void takeDamage(int damage);
  // give it a string representation of the entity
  virtual auto toString() const -> std::string = 0;
  friend void attack(Entity &attacker, Entity &defender);
};

#endif
