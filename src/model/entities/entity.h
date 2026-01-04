#ifndef ENTITY_H
#define ENTITY_H

#include "utils/game_settings.h"
#include "utils/point.h"
#include <string>

class Entity {

public:
  explicit Entity(const Point &position, CellType cellType = CellType::EMPTY);
  Entity();
  virtual ~Entity();

  // Other member functions
  virtual void move(const Point &destination) = 0;
  virtual std::string toString() const = 0;

  // data
  Point position;
  CellType cellType;
};

#endif // ENTITY_H
