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
    : Monster(_position, GameSettings::goblinHealth, GameSettings::goblinDamage,
              Represetiation(Symbols::goblin, Colors::goblin)) {}

void Goblin::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

auto Goblin::toString() const -> std::string { return "Goblin"; }

Orc::Orc(const Point &_position)
    : Monster(_position, GameSettings::orcHealth, GameSettings::orcDamage,
              Represetiation(Symbols::orc, Colors::orc)) {}

void Orc::move(int /*dx*/, int /*dy*/) {
  if (!path.empty()) {
    position = path.front();
    path.pop_front();
  }
}

auto Orc::toString() const -> std::string { return "Orc"; }

void Orc::setPath(const std::deque<Point> &_path) { path = _path; }

auto Orc::isPathEmpty() const -> bool { return path.empty(); }

Troll::Troll(const Point &_position)
    : Monster(_position, GameSettings::trollHealth, GameSettings::trollDamage,
              Represetiation(Symbols::troll, Colors::troll)) {
  velocity = Point(1, 1);
}

void Troll::move(int dx, int dy) {
  Entity::move(dx, dy);
  randomizeVelocity();
}

auto Troll::toString() const -> std::string { return "Troll"; }

Dragon::Dragon(const Point &_position)
    : Monster(_position, GameSettings::dragonHealth, GameSettings::dragonDamage,
              Represetiation(Symbols::dragon, Colors::dragon)) {
  velocity = Point(0, 0);
}

void Dragon::move(int /*dx*/, int /*dy*/) {}

auto Dragon::toString() const -> std::string { return "Dragon"; }
