#include "treasure.h"
#include "utils/game_settings.h"
#include <random>

Treasure::Treasure(const Point &_position, int multiplier)
    : Entity(_position, 0, 0,
             Represetiation(Symbols::treasure, Colors::treasure)) {
  /**
   * @brief Constructor for Treasure class.
   * @param _position Position of the entity.
   * @param multiplier Multiplier of the treasure.
   * @return Treasure object.
   */

  auto val = rand() % 3;
  switch (val) {
  case 0:
    bonus.health = GameSettings::treasureHealth * pow(1.25, multiplier);
    break;
  case 1:
    bonus.attack = GameSettings::treasureAttack * pow(1.25, multiplier);
    break;
  case 2:
    bonus.exp = GameSettings::treasureExperience * pow(1.25, multiplier);
    break;
  }
}

auto Treasure::toString() const -> std::string {
  /**
   * @brief Get the string representation of the treasure.
   * @return String representation of the treasure.
   */
  return "Treasure";
}

auto Treasure::getBonus() const -> Bonus {
  /**
   * @brief Get the bonus of the treasure.
   * @return Bonus of the treasure.
   */
  return bonus;
}
