#include "player.h"
#include "game_settings.h"
#include <cmath>

Player::Player(const Point &_position, int _health, int _attack)
    : Entity(_position, _health, _attack,
             Represetiation(Symbols::player, Colors::player)) {
  /**
   * @brief Constructor for Player class.
   * @param _position Position of the entity.
   * @param _health Health of the entity.
   * @param _attack Attack of the entity.
   * @return Player object.
   */
  level = 1;
  exp = 0;
}
Player::Player(const Point &_position) : Player() {
  /**
   * @brief Constructor for Player class.
   * @param _position Position of the entity.
   * @return Player object.
   */
  position = Point(_position);
}

Player::Player()
    : Entity(Point(), GameSettings::playerHealth, GameSettings::playerDamage,
             Represetiation(Symbols::player, Colors::player)) {
  /**
   * @brief Default constructor for Player class.
   * @return Player object.
   */
  level = 1;
  exp = 0;
}

void Player::levelUp() {
  /**
   * @brief Increase the level of the player.
   * @return Nothing.
   */
  if (!isLevelUp()) {
    return;
  }

  level++;
  exp = 0;
  setHealth(maxHealth * pow(1.1, level - 1));

  if (isLevelUp()) {
    levelUp();
  }
}

auto Player::expToNextLevel() const -> int {
  /**
   * @brief Get the experience needed to reach the next level.
   * @return Experience needed to reach the next level.
   */
  return level * 100 * pow(1.25, level - 1);
}

auto Player::isLevelUp() const -> bool {
  /**
   * @brief Check if the player has reached the next level.
   * @return True if the player has reached the next level, false otherwise.
   */
  return exp >= expToNextLevel();
}

void Player::setExp(int _exp) {
  /**
   * @brief Set the experience of the player.
   * @param _exp Experience of the player.
   * @return Nothing.
   */
  exp = _exp;
  if (isLevelUp()) {
    levelUp();
  }
}

auto Player::getLevel() const -> int {
  /**
   * @brief Get the level of the player.
   * @return Level of the player.
   */
  return level;
}

auto Player::getExp() const -> int {
  /**
   * @brief Get the experience of the player.
   * @return Experience of the player.
   */
  return exp;
}

auto Player::toString() const -> std::string {
  /**
   * @brief Get the representation of the player.
   * @return Representation of the player.
   */
  return "Player";
}
