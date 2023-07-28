#include "movable_entity.h"

MovableEntity::MovableEntity(CellType _cellType, int _health, int _strength,
                             const Point &_position, const Point &_velocity)
    : Entity(_position, _cellType), health(_health), strength(_strength),
      velocity(_velocity) {}

MovableEntity::~MovableEntity() {}

bool MovableEntity::isAlive() const { return health > 0; }

void MovableEntity::takeDamage(int damage) {
  health -= damage;
  if (health < 0) {
    health = 0;
  }
}

void MovableEntity::move(const Point &destination) { position = destination; }

Point MovableEntity::getVelocity() { return velocity; }
