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
  unsigned int width;
  unsigned int height;

public:
  Map(unsigned int _width, unsigned int _height);
  void draw(const Player &player);
  char getChar(Point point);
  int screenWidth();
  int screenHeight();
  void clear();
  void loadLevel();
  bool isPositionFree(Point point);
  Point randomFreePosition();
  Point getStart();
  Point getEnd();
};

#endif
