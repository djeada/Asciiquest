#include "monster.h"
#include "utils.h"

Monster::Monster(const Point &_position, int _health, int _attack,
                 Represetiation _representation)
    : Entity(_position, _health, _attack, _representation) {}

void Monster::randomizeVelocity() {
  int dx = rand() % 3 - 1;
  int dy = rand() % 3 - 1;
  if (dx == 0 && dy == 0) {
    {
      { randomizeVelocity(); }
    }
  } else {
    velocity.x *= dx;
    velocity.y *= dy;
  }
}

Goblin::Goblin(const Point &_position)
    : Monster(_position, 10, 2, Represetiation('G', MONSTER_COLOR)) {}

void Goblin::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

Orc::Orc(const Point &_position)
    : Monster(_position, 20, 3, Represetiation('O', MONSTER_COLOR)) {}

void Orc::move(int dx, int dy) {
  Entity::move(dx, dy);
  // TODO(adam): implement orc movement
}

Troll::Troll(const Point &_position)
    : Monster(_position, 30, 4, Represetiation('T', MONSTER_COLOR)) {
  velocity = Point(3, 3);
}

void Troll::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

Dragon::Dragon(const Point &_position)
    : Monster(_position, 40, 5, Represetiation('D', MONSTER_COLOR)) {
  velocity = Point(0, 0);
}

void Dragon::move(int /*dx*/, int /*dy*/) {}
