
// the purpose of this file is to define a class that find a best route
// between two given points on a map.
// map is represented by a vector of strings.
// the map is not stored within the class, only a pointer to it
// the path is a vector of points
#include "model/map.h"
#include <deque>
#include <vector>

class AStar {
  /**
   * @brief Find the best route between two given points on a map.
   *
   */
private:
  std::deque<Point> bestPath;
  void solveAStar(const Map &map, const Point &start, const Point &end);

public:
  AStar(const Map &map, const Point &start, const Point &end);
  auto getPath() const -> std::deque<Point>;
};
