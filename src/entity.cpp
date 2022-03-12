#include "entity.h"
#include "game_settings.h"
#include <algorithm>
#include <ncurses.h>

// TODO(adam): convert attack to strength

Entity::Entity(const Point &_position, int _health, int _attack,
               Represetiation _representation)
    : position(_position), health(_health), strength(_attack),
      representation(_representation) {
  maxHealth = _health;
  velocity = Point(1, 1);
}

Entity::Entity()
    : position(Point(0, 0)), health(0), strength(0),
      representation(Represetiation(Symbols::player, Colors::player)) {
  maxHealth = health;
  velocity = Point(1, 1);
}

Entity::~Entity() {}

auto Entity::getPosition() const -> Point { return position; }

auto Entity::getHealth() const -> int { return health; }

auto Entity::getMaxHealth() const -> int { return maxHealth; }

auto Entity::getAttack() const -> int { return strength; }

auto Entity::getSymbol() const -> char { return representation.symbol; }

auto Entity::isAlive() const -> bool { return health > 0; }

void Entity::setPosition(const Point &_position) { position = _position; }

void Entity::setHealth(int _health) {
  health = _health;
  maxHealth = std::max(_health, maxHealth);
}

void Entity::setAttack(int _attack) { strength = _attack; }

void Entity::move(int dx, int dy) {
  position.x += dx * velocity.x;
  position.y += dy * velocity.y;
}

void Entity::draw() {
  attron(COLOR_PAIR(representation.color));
  const char symbol = representation.symbol;
  mvprintw(position.y, position.x, &symbol);
  attroff(COLOR_PAIR(representation.color));
}

void Entity::takeDamage(int damage) { health -= damage; }

void attack(Entity &attacker, Entity &defender) {
  defender.takeDamage(attacker.getAttack());
}
