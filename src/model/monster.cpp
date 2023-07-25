#include "monster.h"
#include "utils/game_settings.h"
#include <chrono>
#include <random>

// Improved RNG seeding method
std::mt19937 generateSeededRNG() {
  std::random_device rd;
  std::mt19937 gen(rd());
  return gen;
}

Monster::Monster(const Point &position, int health, int attack)
    : Entity(position, health, attack) {}

void Monster::randomizeVelocity() {
  static std::mt19937 gen = generateSeededRNG();
  std::uniform_int_distribution<> distrib(-1, 1);

  do {
    velocity.x = distrib(gen);
    velocity.y = distrib(gen);
  } while (velocity.x == 0 && velocity.y == 0);
}

std::string Monster::toString() const { return "Monster"; }

Goblin::Goblin(const Point &position)
    : Monster(position, GameSettings::goblinHealth,
              GameSettings::goblinDamage) {}

void Goblin::move(const Point &destination) {
  Entity::move(destination);
  Monster::randomizeVelocity();
}

void Goblin::moveBy(const Point &offset) {
  Entity::moveBy(offset);
  Monster::randomizeVelocity();
}

std::string Goblin::toString() const { return "Goblin"; }

Orc::Orc(const Point &position)
    : Monster(position, GameSettings::orcHealth, GameSettings::orcDamage) {}

void Orc::move(const Point &destination) {
  if (!path.empty()) {
    position = path.front();
    path.pop_front();
  }
}

std::string Orc::toString() const { return "Orc"; }

void Orc::setPath(const std::deque<Point> &path) { this->path = path; }

bool Orc::isPathEmpty() const { return path.empty(); }

Troll::Troll(const Point &position)
    : Monster(position, GameSettings::trollHealth, GameSettings::trollDamage) {
  velocity = Point(1, 1);
}

void Troll::move(const Point &destination) {
  Entity::move(destination);
  randomizeVelocity();
}

std::string Troll::toString() const { return "Troll"; }

Dragon::Dragon(const Point &position)
    : Monster(position, GameSettings::dragonHealth,
              GameSettings::dragonDamage) {
  velocity = Point(0, 0);
}

void Dragon::move(const Point &destination) {
  // Dragon does not move.
}

std::string Dragon::toString() const { return "Dragon"; }
