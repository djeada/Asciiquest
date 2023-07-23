#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_PLAYER_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_PLAYER_H

#include "entity.h"

class Player : public Entity {
  /**
   * @brief Base class for all players used in the game.
   * @details This class is used to represent all players in the game.
   *          It contains crucial information about the player such as
   *          the position, health, attack and representation of the
   *          player, as well as the common functions.
   */
private:
  int level;
  int exp;
  void levelUp();
  auto isLevelUp() const -> bool;

public:
  Player(const Point &_position, int _health, int _attack);
  explicit Player(const Point &_position);
  Player();
  auto getLevel() const -> int;
  auto getExp() const -> int;
  auto expToNextLevel() const -> int;
  void setExp(int _exp);
  Point nextMove();
  auto toString() const -> std::string override;
  // void exploreTreasure(const Treasure &treasure);
};

#endif
