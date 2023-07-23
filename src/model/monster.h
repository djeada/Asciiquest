#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MONSTER_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MONSTER_H

#include "entity.h"
#include "map.h"
#include "utils/game_settings.h"
#include <ncurses.h>

class Monster : public Entity {
  /**
   * @brief Base class for all monsters used in the game.
   * @details This class is used to represent all monsters in the game.
   *          It contains crucial information about the monster such as
   *          the position, health, attack and representation of the
   *          monster, as well as the common functions.
   */
public:
  Monster(const Point &_position, int _health, int _attack,
          Represetiation _representation);
  void randomizeVelocity();
  Point nextMove();
  auto toString() const -> std::string override;
};

class Goblin : public Monster {
  /**
   * @brief Class for Goblin.
   * @details This class is used to represent Goblin in the game.
   *          It contains crucial information about the Goblin such as
   *          the position, health, attack and representation of the
   *          Goblin, as well as the common functions.
   */
public:
  explicit Goblin(const Point &_position);
  void move(Point point) override;
  auto toString() const -> std::string override;
};

class Orc : public Monster {
  /**
   * @brief Class for Orc.
   * @details This class is used to represent Orc in the game.
   *          It contains crucial information about the Orc such as
   *          the position, health, attack and representation of the
   *          Orc, as well as the common functions.
   */
  std::deque<Point> path;

public:
  explicit Orc(const Point &_position);
  void move(Point point) override;
  auto toString() const -> std::string override;
  void setPath(const std::deque<Point> &_path);
  auto isPathEmpty() const -> bool;
};

class Troll : public Monster {
  /**
   * @brief Class for Troll.
   * @details This class is used to represent Troll in the game.
   *          It contains crucial information about the Troll such as
   *          the position, health, attack and representation of the
   *          Troll, as well as the common functions.
   */
public:
  explicit Troll(const Point &_position);
  void move(Point point) override;
  auto toString() const -> std::string override;
};

class Dragon : public Monster {
  /**
   * @brief Class for Dragon.
   * @details This class is used to represent Dragon in the game.
   *          It contains crucial information about the Dragon such as
   *          the position, health, attack and representation of the
   *          Dragon, as well as the common functions.
   */
public:
  explicit Dragon(const Point &_position);
  void move(Point point) override;
  auto toString() const -> std::string override;
};

#endif
