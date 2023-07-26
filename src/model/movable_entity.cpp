#include "movable_entity.h"

MovableEntity::MovableEntity(CellType _cellType, int _health, int _strength,
                             const Point &_position, const Point &_velocity)
    : Entity(_position, _cellType), health(_health), strength(_strength),
      velocity(_velocity) {}

MovableEntity::~MovableEntity() {}

int MovableEntity::getHealth() const { return health; }

int MovableEntity::getAttack() const { return strength; }

Point MovableEntity::getVelocity() const { return velocity; }

bool MovableEntity::isAlive() const { return health > 0; }

void MovableEntity::takeDamage(int damage) { health -= damage; }

void MovableEntity::move(const Point &destination) { position = destination; }
