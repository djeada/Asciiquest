#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_TREASURE_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_TREASURE_H

#include "entity.h"

union Bonus {
  /**
   * @brief Union for Bonus.
   * @details This union is used to represent the bonus of the treasure.
   *          It contains the information about the bonus such as
   *          the type of the bonus.
   */
  int health;
  int attack;
  int exp;
};

class Treasure : public Entity {
  /**
   * @brief Base class for all treasures used in the game.
   * @details This class is used to represent all treasures in the game.
   *          It contains crucial information about the treasure such as
   *          the position, health, attack and representation of the
   *          treasure, as well as the common functions.
   */
  Bonus bonus;

public:
  Treasure(const Point &_position, int multiplier);
 
  auto toString() const -> std::string override;
  auto getBonus() const -> Bonus;
};

#endif
