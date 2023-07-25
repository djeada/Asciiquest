#ifndef MAP_H
#define MAP_H

#include "algorithms/maze_generator.h"
#include "utils/game_settings.h"
#include "utils/point.h"
#include <random>
#include <vector>
class Map {
public:
  std::vector<std::vector<CellType>> grid;

  Map(unsigned int width, unsigned int height);
  void loadLevel();
  void clear();
  CellType getCellType(const Point &point) const;
  void setCellType(const Point &point, CellType cellType);
  bool isPositionFree(const Point &point) const;
  Point randomFreePosition() const;
  Point getStart() const;
  Point getEnd() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;
  std::vector<Point> getNeighbours(const Point &point) const;
  double distance(const Point &point1, const Point &point2) const;
  bool isValidPoint(const Point &point) const;

private:
  mutable std::mt19937 rng;
  unsigned int width;
  unsigned int height;
  Point start;
  Point end;

  std::vector<std::vector<CellType>>
  transformToGrid(const std::vector<std::string> &maze) const;
};

#endif // MAP_H
