#include "entity.h"

Entity::Entity() : underlyingCell(CellType::FLOOR) {}

Entity::Entity(const Point &_position, CellType _cellType)
    : position(_position), cellType(_cellType),
      underlyingCell(CellType::FLOOR) {}

Entity::~Entity() {}
