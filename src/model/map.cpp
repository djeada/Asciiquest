#include "map.h"
#include <algorithm>
#include <array>
#include <queue>
#include <random>

Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

void Map::loadLevel() {
  MazeGenerator generator(width, height,
                          MazeGeneratorAlgorithm::BSP);
  auto maze = generator.getMaze();

  start = {static_cast<int>(generator.getStart().first),
           static_cast<int>(generator.getStart().second)};
  end = {static_cast<int>(generator.getEnd().first),
         static_cast<int>(generator.getEnd().second)};

  // Convert maze to grid with CellType values
  grid = transformToGrid(maze, start, end);
}

void Map::clear() {
  // Initialize grid if it's empty
  if (grid.empty() || grid[0].empty()) {
    grid = std::vector<std::vector<CellType>>(height, std::vector<CellType>(width, CellType::EMPTY));
  } else {
    for (auto &row : grid) {
      std::fill(row.begin(), row.end(), CellType::EMPTY);
    }
  }
}

bool Map::isPositionFree(const Point &point) const {
  if (!isValidPoint(point)) {
    return false;
  }
  
  CellType type = getCellType(point);
  // Allow movement through floor, doors, empty spaces, grass, trees, and desert
  return type == CellType::EMPTY || type == CellType::FLOOR ||
         type == CellType::DOOR || type == CellType::GRASS || 
         type == CellType::TREE || type == CellType::DESERT;
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
Map::transformToGrid(const std::vector<std::string> &maze, const Point &startPoint,
                     const Point &endPoint) const {
  std::vector<std::vector<CellType>> grid;
  grid.reserve(maze.size());

  // Convert maze characters to CellType - preserve BSP room structure exactly
  for (size_t rowIndex = 0; rowIndex < maze.size(); ++rowIndex) {
    const auto &row = maze[rowIndex];
    std::vector<CellType> gridRow;
    gridRow.reserve(row.size());
    for (size_t colIndex = 0; colIndex < row.size(); ++colIndex) {
      const auto &cell = row[colIndex];
      if (cell == '#') {
        gridRow.push_back(CellType::WALL);
      } else {
        gridRow.push_back(CellType::FLOOR);
      }
    }
    grid.push_back(gridRow);
  }

  // Add doors at corridor entrances (narrow passages between walls)
  std::uniform_int_distribution<int> doorChance(0, 99);
  for (size_t y = 1; y < grid.size() - 1; ++y) {
    for (size_t x = 1; x < grid[y].size() - 1; ++x) {
      if (grid[y][x] != CellType::FLOOR) continue;
      
      // Check if this is a doorway (narrow passage between walls)
      bool isVerticalDoor = 
          grid[y][x-1] == CellType::WALL && grid[y][x+1] == CellType::WALL &&
          (grid[y-1][x] == CellType::FLOOR || grid[y+1][x] == CellType::FLOOR);
      bool isHorizontalDoor = 
          grid[y-1][x] == CellType::WALL && grid[y+1][x] == CellType::WALL &&
          (grid[y][x-1] == CellType::FLOOR || grid[y][x+1] == CellType::FLOOR);
      
      if (isVerticalDoor || isHorizontalDoor) {
        // Place door with 40% chance at corridor entrances
        if (doorChance(rng) < 40) {
          grid[y][x] = CellType::DOOR;
        }
      }
    }
  }

  // Validate path exists from start to end
  auto isWalkable = [&grid](const Point &p) {
    CellType type = grid[p.y][p.x];
    return type == CellType::FLOOR || type == CellType::DOOR;
  };

  // Simple path validation using BFS
  if (isValidPoint(startPoint) && isValidPoint(endPoint)) {
    std::queue<Point> frontier;
    std::vector<std::vector<bool>> visited(
        grid.size(), std::vector<bool>(grid[0].size(), false));
    const std::array<Point, 4> directions = {
        Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};

    if (isWalkable(startPoint)) {
      frontier.push(startPoint);
      visited[startPoint.y][startPoint.x] = true;
    }

    bool found = false;
    while (!frontier.empty() && !found) {
      Point current = frontier.front();
      frontier.pop();
      if (current.x == endPoint.x && current.y == endPoint.y) {
        found = true;
        break;
      }
      for (const auto &dir : directions) {
        Point next{current.x + dir.x, current.y + dir.y};
        if (!isValidPoint(next) || visited[next.y][next.x]) {
          continue;
        }
        if (!isWalkable(next)) {
          continue;
        }
        visited[next.y][next.x] = true;
        frontier.push(next);
      }
    }
    
    // If no path found, carve a direct path (fallback)
    if (!found) {
      int x = startPoint.x;
      int y = startPoint.y;
      while (x != endPoint.x) {
        grid[y][x] = CellType::FLOOR;
        x += (endPoint.x > x) ? 1 : -1;
      }
      while (y != endPoint.y) {
        grid[y][x] = CellType::FLOOR;
        y += (endPoint.y > y) ? 1 : -1;
      }
    }
  }

  return grid;
}

unsigned int Map::getWidth() const { return width; }

unsigned int Map::getHeight() const { return height; }
