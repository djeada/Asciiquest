#include "monster.h"

Monster::Monster(const Point &_position, int _health, int _attack,
                 Represetiation _representation)
    : Entity(_position, _health, _attack, _representation) {
  /**
   * @brief Constructor for Monster class.
   * @param _position Position of the entity.
   * @param _health Health of the entity.
   * @param _attack Attack of the entity.
   * @param _representation Symbol and color used to represent the entity.
   * @return Monster object.
   */
}

void Monster::randomizeVelocity() {
  /**
   * @brief Randomize the velocity of the monster.
   * @return Nothing.
   */
  int dx = 0;
  int dy = 0;

  while (dx == 0 && dy == 0) {
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

auto Monster::toString() const -> std::string {
  /**
   * @brief Get the string representation of the monster.
   * @return String representation of the monster.
   */
  return "Monster";
}

Goblin::Goblin(const Point &_position)
    : Monster(_position, GameSettings::goblinHealth, GameSettings::goblinDamage,
              Represetiation(Symbols::goblin, Colors::goblin)) {
  /**
   * @brief Constructor for Goblin class.
   * @param _position Position of the entity.
   * @return Goblin object.
   */
}

void Goblin::move(Point point) {
  /**
   * @brief Move the monster.
   * @param dx offset in x direction.
   * @param dy offset in y direction.
   * @return Nothing.
   */
  Entity::move(point);
  randomizeVelocity();
}

auto Goblin::toString() const -> std::string {
  /**
   * @brief Get the string representation of the goblin.
   * @return String representation of the goblin.
   */
  return "Goblin";
}

Orc::Orc(const Point &_position)
    : Monster(_position, GameSettings::orcHealth, GameSettings::orcDamage,
              Represetiation(Symbols::orc, Colors::orc)) {
  /**
   * @brief Constructor for Orc class.
   * @param _position Position of the entity.
   * @return Orc object.
   */
}

void Orc::move(Point point) {
  /**
   * @brief Move the orc according to the path member variable.
   * @param dx unused.
   * @param dy unused.
   * @return Nothing.
   */
  if (!path.empty()) {
    position = path.front();
    path.pop_front();
  }
}

auto Orc::toString() const -> std::string {
  /**
   * @brief Get the string representation of the orc.
   * @return String representation of the orc.
   */
  return "Orc";
}

void Orc::setPath(const std::deque<Point> &_path) {
  /**
   * @brief Set the path the orc will follow.
   * @param _path Path of the orc.
   * @return Nothing.
   */
  path = _path;
}

auto Orc::isPathEmpty() const -> bool {
  /**
   * @brief Check if the orc has a path.
   * @return True if the orc has a path, false otherwise.
   */
  return path.empty();
}

Troll::Troll(const Point &_position)
    : Monster(_position, GameSettings::trollHealth, GameSettings::trollDamage,
              Represetiation(Symbols::troll, Colors::troll)) {
  /**
   * @brief Constructor for Troll class.
   * @param _position Position of the entity.
   * @return Troll object.
   */
  velocity = Point(1, 1);
}

void Troll::move(Point point) {
  /**
   * @brief Move the troll.
   * @param dx offset in x direction.
   * @param dy offset in y direction.
   * @return Nothing.
   */
  Entity::move(point);
  randomizeVelocity();
}

auto Troll::toString() const -> std::string {
  /**
   * @brief Get the string representation of the troll.
   * @return String representation of the troll.
   */
  return "Troll";
}

Point Monster::nextMove() { return Point(); }

Dragon::Dragon(const Point &_position)
    : Monster(_position, GameSettings::dragonHealth, GameSettings::dragonDamage,
              Represetiation(Symbols::dragon, Colors::dragon)) {
  /**
   * @brief Constructor for Dragon class.
   * @param _position Position of the entity.
   * @return Dragon object.
   */
  velocity = Point(0, 0);
}

void Dragon::move(Point point) {
  /**
   * @brief Implemented out of necessity. Dragon does not move.
   * @param dx unused.
   * @param dy unused.
   * @return Nothing.
   */
}

auto Dragon::toString() const -> std::string {
  /**
   * @brief Get the string representation of the dragon.
   * @return String representation of the dragon.
   */
  return "Dragon";
}
