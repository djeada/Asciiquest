#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MAP_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MAP_H

#include "maze_generator.h"
#include "player.h"
#include "utils.h"
#include <string>
#include <vector>

struct PlayerInfo {
  std::string health;
  std::string maxHealth;
  std::string level;
  std::string exp;
  std::string nextExp;

  PlayerInfo(const Player &player) {
    health = std::to_string(player.getHealth());
    maxHealth = std::to_string(player.getMaxHealth());
    level = std::to_string(player.getLevel());
    exp = std::to_string(player.getExp());
    nextExp = std::to_string(player.expToNextLevel());
  }
};

class Map {
private:
  std::vector<std::string> map;
  Point start;
  Point end;
  unsigned int width;
  unsigned int height;
  std::vector<std::string> fightInfo;

public:
  Map(unsigned int _width, unsigned int _height);
  void draw(const Player &player);
  auto getChar(const Point &point) -> char;
  auto screenWidth() const -> int;
  auto screenHeight() const -> int;
  void clear();
  void loadLevel();
  auto isPositionFree(const Point &point) -> bool;
  auto randomFreePosition() -> Point;
  auto getStart() -> Point;
  auto getEnd() -> Point;
  void setFightInfo(const std::vector<std::string> &info);
};

#endif
