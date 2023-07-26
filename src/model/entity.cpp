#include "entity.h"

Entity::Entity() {}

Entity::Entity(const Point &_position, CellType _cellType)
    : position(_position), cellType(_cellType) {}

Entity::~Entity() {}
