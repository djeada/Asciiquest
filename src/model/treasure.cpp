#include "treasure.h"
#include "utils/game_settings.h"
#include <random>

Treasure::Treasure() : Entity() { value = 0; }

Treasure::Treasure(const Point &_position, int _value)
    : Entity(_position, CellType::EMPTY), value(_value) {}

int Treasure::getValue() const { return value; }

void Treasure::setValue(int _value) { value = _value; }

auto Treasure::toString() const -> std::string { return "Treasure"; }
