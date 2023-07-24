#ifndef _MAP_H
#define _MAP_H

#include "algorithms/maze_generator.h"
#include "utils/game_settings.h"
#include "utils/point.h"
#include <string>
#include <vector>

class Map {
public:
  Map(unsigned int _width, unsigned int _height);

  void loadLevel();
  void clear();
  CellType getChar(const Point &point) const;
  void setChar(const Point &point, CellType symbol);
  int screenWidth() const;
  int screenHeight() const;
  bool isPositionFree(const Point &point) const;
  Point randomFreePosition() const;
  Point getStart() const;
  Point getEnd() const;
  std::vector<Point> getNeighbours(const Point &point) const;
  double distance(const Point &point1, const Point &point2) const;
  std::vector<std::vector<CellType>> grid;
  CellType getCellType(const Point &point) const;

private:
  unsigned int width;
  unsigned int height;

  Point start;
  Point end;
};

#endif
