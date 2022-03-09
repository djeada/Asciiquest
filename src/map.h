#ifndef map_h
#define map_h

#include "maze_generator.h"
#include "player.h"
#include "utils.h"
#include <string>
#include <vector>

class Map {
private:
  std::vector<std::string> map;
  Point start;
  Point end;

public:
  Map();
  void draw(const Player &player);
  char getChar(Point point);
  int screenWidth();
  int screenHeight();
  void clear();
  void loadLevel();
  Point randomFreePosition();
  Point getStart();
  Point getEnd();
};

#endif
