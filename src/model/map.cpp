#include "map.h"
#include "utils/global_config.h"
#include <algorithm>
#include <random>

Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

void Map::loadLevel() {
  MazeGenerator generator(width, height,
                          MazeGeneratorAlgorithm::RandomizedPrim);
  auto maze = generator.getMaze();

  // Convert maze to grid with CellType values
  grid = transformToGrid(maze);

  start = {static_cast<int>(generator.getStart().first),
           static_cast<int>(generator.getStart().second)};
  end = {static_cast<int>(generator.getEnd().first),
         static_cast<int>(generator.getEnd().second)};
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
  // Allow movement through empty spaces, grass, trees, and desert
  return type == CellType::EMPTY || type == CellType::GRASS || 
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
Map::transformToGrid(const std::vector<std::string> &maze) const {
  std::vector<std::vector<CellType>> grid;
  grid.reserve(maze.size());

  // Use existing rng for terrain placement
  std::uniform_int_distribution<int> terrainDist(0, 99);
  int wallCarveChance =
      GlobalConfig::getInstance().getConfig<int>("WallCarveChance");
  
  for (size_t rowIndex = 0; rowIndex < maze.size(); ++rowIndex) {
    const auto &row = maze[rowIndex];
    std::vector<CellType> gridRow;
    gridRow.reserve(row.size());
    for (size_t colIndex = 0; colIndex < row.size(); ++colIndex) {
      const auto &cell = row[colIndex];
      if (cell == '#') {
        bool isBorder = rowIndex == 0 || colIndex == 0 ||
                        rowIndex == maze.size() - 1 ||
                        colIndex == row.size() - 1;
        int carveRoll = terrainDist(rng);
        if (!isBorder && carveRoll < wallCarveChance) {
          gridRow.push_back(CellType::EMPTY);
        } else {
          // For walls, randomly place mountains or keep as walls
          int chance = terrainDist(rng);
          if (chance < 25) {
            gridRow.push_back(CellType::MOUNTAIN);
          } else {
            gridRow.push_back(CellType::WALL);
          }
        }
      } else {
        // For empty spaces, randomly place various terrain types
        int chance = terrainDist(rng);
        if (chance < 15) {
          gridRow.push_back(CellType::GRASS);
        } else if (chance < 25) {
          gridRow.push_back(CellType::TREE);
        } else if (chance < 35) {
          gridRow.push_back(CellType::WATER);
        } else if (chance < 40) {
          gridRow.push_back(CellType::DESERT);
        } else {
          gridRow.push_back(CellType::EMPTY);
        }
      }
    }
    grid.push_back(gridRow);
  }

  int totalCells = static_cast<int>(maze.size() * maze[0].size());
  int maxWallCells = static_cast<int>(totalCells * 0.30);
  int wallCells = 0;
  std::vector<Point> wallCandidates;
  wallCandidates.reserve(totalCells / 2);

  for (size_t y = 0; y < grid.size(); ++y) {
    for (size_t x = 0; x < grid[y].size(); ++x) {
      CellType type = grid[y][x];
      if (type == CellType::WALL || type == CellType::MOUNTAIN) {
        wallCells++;
        bool isBorder = y == 0 || x == 0 || y == grid.size() - 1 ||
                        x == grid[y].size() - 1;
        if (!isBorder) {
          wallCandidates.emplace_back(static_cast<int>(x),
                                      static_cast<int>(y));
        }
      }
    }
  }

  if (wallCells > maxWallCells && !wallCandidates.empty()) {
    std::shuffle(wallCandidates.begin(), wallCandidates.end(), rng);
    int toCarve = wallCells - maxWallCells;
    int carved = 0;
    for (const auto &pos : wallCandidates) {
      if (carved >= toCarve) {
        break;
      }
      grid[pos.y][pos.x] = CellType::EMPTY;
      carved++;
    }
  }

  return grid;
}

unsigned int Map::getWidth() const { return width; }

unsigned int Map::getHeight() const { return height; }
