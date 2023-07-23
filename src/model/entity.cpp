#include "entity.h"
#include "utils/game_settings.h"
#include <algorithm>
#include <ncurses.h>

// TODO(adam): convert attack to strength

Entity::Entity(const Point &_position, int _health, int _attack,
               Represetiation _representation)
    : position(_position), health(_health), strength(_attack),
      representation(_representation) {
  /**
   * @brief Constructor for Entity class.
   * @param _position Position of the entity.
   * @param _health Health of the entity.
   * @param _attack Attack of the entity.
   * @param _representation Symbol and color used to represent the entity.
   * @return Entity object.
   */
  maxHealth = _health;
  velocity = Point(1, 1);
}

Entity::Entity()
    : position(Point(0, 0)), health(0), strength(0),
      representation(Represetiation(Symbols::player, Colors::player)) {
  /**
   * @brief Constructor for Entity class.
   * @param _position Position of the entity.
   * @param _health Health of the entity.
   * @param _attack Attack of the entity.
   * @param _representation Symbol and color used to represent the entity.
   * @return Entity object.
   */
  maxHealth = health;
  velocity = Point(1, 1);
}

Entity::~Entity() {
  /**
   * @brief Destructor for Entity class.
   * @return Nothing.
   */
}

auto Entity::getPosition() const -> Point {
  /**
   * @brief Get the position of the entity.
   * @return Position of the entity.
   */
  return position;
}

auto Entity::getHealth() const -> int {
  /**
   * @brief Get the health of the entity.
   * @return Health of the entity.
   */
  return health;
}

auto Entity::getMaxHealth() const -> int {
  /**
   * @brief Get the maximum health of the entity.
   * @return Maximum health of the entity.
   */
  return maxHealth;
}

auto Entity::getAttack() const -> int {
  /**
   * @brief Get the attack of the entity.
   * @return Attack of the entity.
   */
  return strength;
}

auto Entity::getSymbol() const -> char {
  /**
   * @brief Get the symbol of the entity.
   * @return Symbol of the entity.
   */
  return representation.symbol;
}

auto Entity::isAlive() const -> bool {
  /**
   * @brief Check if the entity is alive.
   * @return True if the entity is alive, false otherwise.
   */
  return health > 0;
}

void Entity::setPosition(const Point &_position) {
  /**
   * @brief Set the position of the entity.
   * @param _position Position of the entity.
   * @return Nothing.
   */
  position = _position;
}

void Entity::setHealth(int _health) {
  /**
   * @brief Set the health of the entity.
   * @param _health Health of the entity.
   * @return Nothing.
   */
  health = _health;
  maxHealth = std::max(_health, maxHealth);
}

void Entity::setAttack(int _attack) {
  /**
   * @brief Set the attack of the entity.
   * @param _attack Attack of the entity.
   * @return Nothing.
   */
  strength = _attack;
}

void Entity::move(Point point) {
  /**
   * @brief Move the entity.
   * @param dx offset in x direction.
   * @param dy offset in y direction.
   * @return Nothing.
   */
  position = point;
}

void Entity::draw() {
  /**
   * @brief Draw the entity using ncurses functions.
   * @return Nothing.
   */
  attron(COLOR_PAIR(representation.color));
  const char symbol = representation.symbol;
  char str[2] = {symbol, '\0'};
  mvprintw(position.y, position.x, str);
  attroff(COLOR_PAIR(representation.color));
}

void Entity::takeDamage(int damage) {
  /**
   * @brief Decrease the health of the entity by the given amount.
   * @param damage Amount of damage taken.
   * @return Nothing.
   */
  health -= damage;
}

void attack(Entity &attacker, Entity &defender) {
  /**
   * @brief Attack the defender.
   * @param attacker Entity attacking.
   * @param defender Entity defending.
   * @return Nothing.
   */
  defender.takeDamage(attacker.getAttack());
}
