#include "entity.h"
#include "game_settings.h"
#include <ncurses.h>

// TODO(adam): convert attack to strength

Entity::Entity(const Point &_position, int _health, int _attack,
               Represetiation _representation)
    : position(_position), health(_health), strength(_attack),
      representation(_representation) {
  velocity = Point(1, 1);
}

Entity::Entity()
    : position(Point(0, 0)), health(0), strength(0),
      representation(Represetiation('@', PLAYER_COLOR)) {
  velocity = Point(1, 1);
}

Entity::~Entity() {}

auto Entity::getPosition() -> Point { return position; }

auto Entity::getHealth() const -> int { return health; }

auto Entity::getAttack() const -> int { return strength; }

auto Entity::getSymbol() const -> char { return representation.symbol; }

auto Entity::isAlive() const -> bool { return health > 0; }

void Entity::setPosition(const Point &_position) { position = _position; }

void Entity::setHealth(int _health) { health = _health; }

void Entity::setAttack(int _attack) { strength = _attack; }

void Entity::move(int dx, int dy) {
  position.x += dx * velocity.x;
  position.y += dy * velocity.y;
}

void Entity::draw() {
  attron(COLOR_PAIR(representation.color));
  mvprintw(position.y, position.x, &representation.symbol);
  attroff(COLOR_PAIR(representation.color));
}

void Entity::takeDamage(int damage) { health -= damage; }

void attack(Entity &attacker, Entity &defender) {
  defender.takeDamage(attacker.getAttack());
}
