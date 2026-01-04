#include "map.h"
#include <algorithm>
#include <random>

Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

void Map::loadLevel() {
  MazeGenerator generator(width, height,
                          MazeGeneratorAlgorithm::DepthFirstSearch);
  auto maze = generator.getMaze();

  // Convert maze to grid with CellType values
  grid = transformToGrid(maze);

  start = {static_cast<int>(generator.getStart().first),
           static_cast<int>(generator.getStart().second)};
  end = {static_cast<int>(generator.getEnd().first),
         static_cast<int>(generator.getEnd().second)};
}

void Map::clear() {
  for (auto &row : grid) {
    std::fill(row.begin(), row.end(), CellType::EMPTY);
  }
}

bool Map::isPositionFree(const Point &point) const {
  return isValidPoint(point) && getCellType(point) == CellType::EMPTY;
}

Point Map::randomFreePosition() const {
  std::uniform_int_distribution<int> dist_x(0, width - 1);
  std::uniform_int_distribution<int> dist_y(0, height - 1);

  Point p;
  do {
    p = {dist_x(rng), dist_y(rng)};
  } while (!isPositionFree(p));

  return p;
}

Point Map::getStart() const { return start; }

Point Map::getEnd() const { return end; }

std::vector<Point> Map::getNeighbours(const Point &point) const {
  std::vector<Point> neighbours;
  neighbours.reserve(8); // Maximum 8 neighbours in a grid

  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;
      Point neighbour{point.x + dx, point.y + dy};
      if (isPositionFree(neighbour))
        neighbours.push_back(neighbour);
    }
  }

  return neighbours;
}

double Map::distance(const Point &point1, const Point &point2) const {
  int dx = std::abs(point1.x - point2.x);
  int dy = std::abs(point1.y - point2.y);

  return dx > 1 || dy > 1     ? std::numeric_limits<double>::infinity()
         : dx == 1 && dy == 1 ? std::sqrt(2)
                              : 1;
}

void Map::setCellType(const Point &point, CellType symbol) {
  if (isValidPoint(point)) {
    grid[point.y][point.x] = symbol;
  } else {
    //  throw std::out_of_range("Point is outside of the map's boundaries.");
  }
}

CellType Map::getCellType(const Point &point) const {
  if (isValidPoint(point)) {
    return grid[point.y][point.x];
  }
  // Return WALL for out-of-bounds points to prevent movement outside map
  return CellType::WALL;
}

bool Map::isValidPoint(const Point &point) const {
  return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height;
}

std::vector<std::vector<CellType>>
Map::transformToGrid(const std::vector<std::string> &maze) const {
  std::vector<std::vector<CellType>> grid;
  grid.reserve(maze.size());

  for (const auto &row : maze) {
    std::vector<CellType> gridRow;
    gridRow.reserve(row.size());
    for (const auto &cell : row) {
      gridRow.push_back(cell == '#' ? CellType::WALL : CellType::EMPTY);
    }
    grid.push_back(gridRow);
  }

  return grid;
}

unsigned int Map::getWidth() const { return width; }

unsigned int Map::getHeight() const { return height; }
