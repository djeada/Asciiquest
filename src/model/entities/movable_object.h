#ifndef MOVABLE_OBJECT_H
#define MOVABLE_OBJECT_H

#include "entity.h"
#include <string>

class MovableObject : public Entity {
private:
  bool pushable;
  int weight;

public:
  explicit MovableObject(CellType cellType = CellType::BOULDER,
                         const Point &position = Point(0, 0),
                         bool pushable = true, int weight = 1);
  ~MovableObject() override;

  bool isPushable() const;
  int getWeight() const;
  void move(const Point &destination) override;
  std::string toString() const override;
};

class Boulder : public MovableObject {
public:
  explicit Boulder(const Point &position = Point(0, 0));
  std::string toString() const override;
};

class Crate : public MovableObject {
public:
  explicit Crate(const Point &position = Point(0, 0));
  std::string toString() const override;
};

class Barrel : public MovableObject {
public:
  explicit Barrel(const Point &position = Point(0, 0));
  std::string toString() const override;
};

#endif // MOVABLE_OBJECT_H
