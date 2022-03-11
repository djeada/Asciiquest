#include "monster.h"
#include "utils.h"

Monster::Monster(const Point &_position, int _health, int _attack,
                 Represetiation _representation)
    : Entity(_position, _health, _attack, _representation) {}

void Monster::randomizeVelocity() {
  int dx = 0;
  int dy = 0;

  while (dx == 0 && dy == 0) {
    // dx = headOrTail() - headOrTail();
    // dy = headOrTail() - headOrTail();
    dx = rand() % 3 - 1;
    dy = rand() % 3 - 1;
  }

  if (velocity.x * dx == 0 && velocity.y * dy == 0) {

    if (velocity.y == 0 && velocity.x != 0)
      velocity.y = velocity.x;
    if (velocity.x == 0 && velocity.y != 0)
      velocity.x = velocity.y;
    return;
  }

  velocity.x *= dx;
  velocity.y *= dy;
}

auto Monster::toString() const -> std::string { return "Monster"; }

Goblin::Goblin(const Point &_position)
    : Monster(_position, 10, 2, Represetiation('G', MONSTER_COLOR)) {}

void Goblin::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

auto Goblin::toString() const -> std::string { return "Goblin"; }

Orc::Orc(const Point &_position)
    : Monster(_position, 20, 3, Represetiation('O', MONSTER_COLOR)) {}

void Orc::move(int dx, int dy) {
  Entity::move(dx, dy);
  // TODO(adam): implement orc movement
}

auto Orc::toString() const -> std::string { return "Orc"; }

Troll::Troll(const Point &_position)
    : Monster(_position, 30, 4, Represetiation('T', MONSTER_COLOR)) {
  velocity = Point(1, 1);
}

void Troll::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

auto Troll::toString() const -> std::string { return "Troll"; }

Dragon::Dragon(const Point &_position)
    : Monster(_position, 40, 5, Represetiation('D', MONSTER_COLOR)) {
  velocity = Point(0, 0);
}

void Dragon::move(int /*dx*/, int /*dy*/) {}

auto Dragon::toString() const -> std::string { return "Dragon"; }