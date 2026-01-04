#include "movable_object.h"

MovableObject::MovableObject(CellType _cellType, const Point &_position,
                             bool _pushable, int _weight)
    : Entity(_position, _cellType), pushable(_pushable), weight(_weight) {}

MovableObject::~MovableObject() {}

bool MovableObject::isPushable() const { return pushable; }

int MovableObject::getWeight() const { return weight; }

void MovableObject::move(const Point &destination) { position = destination; }

std::string MovableObject::toString() const { return "Movable Object"; }

Boulder::Boulder(const Point &_position)
    : MovableObject(CellType::BOULDER, _position, true, 3) {}

std::string Boulder::toString() const { return "Boulder"; }

Crate::Crate(const Point &_position)
    : MovableObject(CellType::CRATE, _position, true, 1) {}

std::string Crate::toString() const { return "Crate"; }

Barrel::Barrel(const Point &_position)
    : MovableObject(CellType::BARREL, _position, true, 2) {}

std::string Barrel::toString() const { return "Barrel"; }
