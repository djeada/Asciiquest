#ifndef MOVABLE_ENTITY_H
#define MOVABLE_ENTITY_H

#include "entity.h"
#include <string>

class MovableEntity : public Entity {
protected:
  Point velocity;

public:
  explicit MovableEntity(CellType cellType = CellType::EMPTY, int health = 100,
                         int strength = 10, const Point &position = Point(0, 0),
                         const Point &velocity = Point(0, 0));
  virtual ~MovableEntity();

  // Other member functions

  bool isAlive() const;
  void takeDamage(int damage);
  virtual Point getVelocity();
  virtual void move(const Point &destination);

  // data
  int health;
  int strength;
};

#endif
