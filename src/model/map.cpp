#include "map.h"

#include <cstdlib> // for rand
#include <fstream>
#include <limits> // for numeric_limits
#include <sstream>

// Constructor
Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

// Load a new map using the maze generator
void Map::loadLevel() {
  MazeGenerator generator(width, height,
                          MazeGeneratorAlgorithm::DepthFirstSearch);

  auto maze = generator.getMaze();

  // Convert maze to grid with CellType values
  for (auto &row : maze) {
    std::vector<CellType> gridRow;
    for (auto &cell : row) {
      gridRow.push_back(cell == '#' ? CellType::WALL : CellType::EMPTY);
    }
    grid.push_back(gridRow);
  }

  start = Point(generator.getStart().first, generator.getStart().second);
  end = Point(generator.getEnd().first, generator.getEnd().second);
}

// Clear the current map
void Map::clear() {
  for (auto &row : grid) {
    std::fill(row.begin(), row.end(), CellType::EMPTY);
  }
}

// Get the CellType at a given point
CellType Map::getCellType(const Point &point) const {
  return grid[point.y][point.x];
}

// Get the width of the screen
int Map::screenWidth() const { return width; }

// Get the height of the screen
int Map::screenHeight() const { return height; }

// Check if a given position is free
bool Map::isPositionFree(const Point &point) const {
  return point.x >= 0 && point.x < screenWidth() && point.y >= 0 &&
         point.y < screenHeight() && getCellType(point) == CellType::EMPTY;
}

// Get a random free position
Point Map::randomFreePosition() const {
  int x = 0;
  int y = 0;

  do {
    x = rand() % screenWidth();
    y = rand() % screenHeight();
  } while (!isPositionFree(Point(x, y)));

  return Point(x, y);
}

// Get the start position
Point Map::getStart() const { return start; }

// Get the end position
Point Map::getEnd() const { return end; }

// Get the neighbours of a given point
std::vector<Point> Map::getNeighbours(const Point &point) const {
  std::vector<Point> neighbours;

  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;
      Point neighbour(point.x + dx, point.y + dy);
      if (isPositionFree(neighbour))
        neighbours.push_back(neighbour);
    }
  }

  return neighbours;
}

// Calculate the distance between two points
double Map::distance(const Point &point1, const Point &point2) const {
  int dx = abs(point1.x - point2.x);
  int dy = abs(point1.y - point2.y);

  if (dx > 1 || dy > 1)
    return std::numeric_limits<double>::max();

  return dx == 1 && dy == 1 ? std::sqrt(2) : 1;
}

void Map::setChar(const Point &point, CellType symbol) {
  // Ensure the point is within the map boundaries before setting the cell value
  if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
    grid[point.y][point.x] = symbol;
  } else {
    // Handle out of bounds case
    // This could be by throwing an exception, or logging an error message,
    // depending on your application's error handling strategy
  }
}

CellType Map::getChar(const Point &point) const {
  // Check if the point is within the map boundaries before getting the cell
  // value
  if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
    return grid[point.y][point.x];
  } else {
    // Handle out of bounds case
    // You might want to throw an exception, return a specific invalid CellType,
    // or implement another form of error handling based on your application's
    // needs
    throw std::out_of_range("Point is outside of the map's boundaries.");
  }
}
