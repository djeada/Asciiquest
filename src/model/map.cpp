#include "map.h"
#include "utils/global_config.h"
#include <algorithm>
#include <array>
#include <queue>
#include <random>

Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

void Map::loadLevel() {
  MazeGenerator generator(width, height,
                          MazeGeneratorAlgorithm::RandomizedPrim);
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
          // Carved walls become floor
          gridRow.push_back(CellType::FLOOR);
        } else {
          // Keep walls consistent - no random mountains
          gridRow.push_back(CellType::WALL);
        }
      } else {
        // Empty spaces become floor tiles (clean dungeon look)
        gridRow.push_back(CellType::FLOOR);
      }
    }
    grid.push_back(gridRow);
  }

  // Post-process: Add doors at corridor entrances (where floor meets wall opening)
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
        // Only place door with 30% chance to avoid too many doors
        if (terrainDist(rng) < 30) {
          grid[y][x] = CellType::DOOR;
        }
      }
    }
  }

  // Carve small rooms along corridors to create pockets for blocking objects
  // These rooms provide space for pushing objects and create meaningful puzzles
  std::vector<Point> corridorCells;
  for (size_t y = 4; y + 4 < grid.size(); ++y) {
    for (size_t x = 4; x + 4 < grid[y].size(); ++x) {
      if (grid[y][x] == CellType::FLOOR) {
        // Check if this is a corridor (not already in an open area)
        int adjacentFloors = 0;
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            if (grid[y + dy][x + dx] == CellType::FLOOR ||
                grid[y + dy][x + dx] == CellType::DOOR) {
              adjacentFloors++;
            }
          }
        }
        // Corridor cells typically have 2-4 adjacent floors
        if (adjacentFloors >= 2 && adjacentFloors <= 4) {
          corridorCells.emplace_back(static_cast<int>(x), static_cast<int>(y));
        }
      }
    }
  }

  // Create rooms at some corridor locations
  int mapArea = static_cast<int>(grid.size() * grid[0].size());
  int numRooms = std::max(3, mapArea / 800);
  numRooms = std::min(numRooms, 8);
  std::shuffle(corridorCells.begin(), corridorCells.end(), rng);
  std::uniform_int_distribution<int> roomSizeDist(3, 5);

  int roomsCreated = 0;
  for (const auto &center : corridorCells) {
    if (roomsCreated >= numRooms) break;

    int roomWidth = roomSizeDist(rng);
    int roomHeight = roomSizeDist(rng);
    int halfW = roomWidth / 2;
    int halfH = roomHeight / 2;

    // Check if room fits and doesn't overlap with start/end
    bool canPlace = true;
    int minY = center.y - halfH;
    int maxY = center.y + halfH;
    int minX = center.x - halfW;
    int maxX = center.x + halfW;

    // Ensure room is within bounds (with margin for walls)
    if (minY < 2 || maxY >= static_cast<int>(grid.size()) - 2 ||
        minX < 2 || maxX >= static_cast<int>(grid[0].size()) - 2) {
      continue;
    }

    // Don't place rooms too close to start or end
    int distToStart = std::abs(center.x - startPoint.x) + std::abs(center.y - startPoint.y);
    int distToEnd = std::abs(center.x - endPoint.x) + std::abs(center.y - endPoint.y);
    if (distToStart < 6 || distToEnd < 6) {
      continue;
    }

    // Check for overlap with existing rooms (look for large floor areas)
    int existingFloors = 0;
    for (int ry = minY; ry <= maxY && canPlace; ++ry) {
      for (int rx = minX; rx <= maxX; ++rx) {
        if (grid[ry][rx] == CellType::FLOOR) {
          existingFloors++;
        }
      }
    }
    // Skip if area is already mostly open
    int roomArea = roomWidth * roomHeight;
    if (existingFloors > roomArea / 2) {
      continue;
    }

    // Carve the room
    for (int ry = minY; ry <= maxY; ++ry) {
      for (int rx = minX; rx <= maxX; ++rx) {
        if (grid[ry][rx] == CellType::WALL) {
          grid[ry][rx] = CellType::FLOOR;
        }
      }
    }
    roomsCreated++;
  }

  int totalCells = static_cast<int>(maze.size() * maze[0].size());
  int maxWallCells = static_cast<int>(totalCells * 0.30);
  int wallCells = 0;
  std::vector<Point> wallCandidates;
  wallCandidates.reserve(totalCells / 2);

  for (size_t y = 0; y < grid.size(); ++y) {
    for (size_t x = 0; x < grid[y].size(); ++x) {
      CellType type = grid[y][x];
      if (type == CellType::WALL) {
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
      grid[pos.y][pos.x] = CellType::FLOOR;
      carved++;
    }
  }

  std::vector<std::vector<bool>> protectedPath(
      grid.size(), std::vector<bool>(grid[0].size(), false));
  auto isWalkable = [&grid](const Point &p) {
    CellType type = grid[p.y][p.x];
    return type == CellType::FLOOR || type == CellType::DOOR ||
           type == CellType::GRASS || type == CellType::TREE ||
           type == CellType::DESERT;
  };

  if (isValidPoint(startPoint) && isValidPoint(endPoint) &&
      isWalkable(startPoint) && isWalkable(endPoint)) {
    std::queue<Point> frontier;
    std::vector<std::vector<bool>> visited(
        grid.size(), std::vector<bool>(grid[0].size(), false));
    std::vector<std::vector<Point>> prev(
        grid.size(), std::vector<Point>(grid[0].size(), {-1, -1}));
    const std::array<Point, 8> directions = {
        Point{1, 0},   Point{-1, 0},  Point{0, 1},  Point{0, -1},
        Point{1, 1},   Point{-1, -1}, Point{1, -1}, Point{-1, 1}};

    frontier.push(startPoint);
    visited[startPoint.y][startPoint.x] = true;

    bool found = false;
    while (!frontier.empty()) {
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
        prev[next.y][next.x] = current;
        frontier.push(next);
      }
    }

    if (found) {
      Point step = endPoint;
      protectedPath[step.y][step.x] = true;
      while (!(step.x == startPoint.x && step.y == startPoint.y)) {
        Point previous = prev[step.y][step.x];
        if (!isValidPoint(previous)) {
          break;
        }
        protectedPath[previous.y][previous.x] = true;
        step = previous;
      }
      protectedPath[startPoint.y][startPoint.x] = true;
    } else {
      protectedPath[startPoint.y][startPoint.x] = true;
      protectedPath[endPoint.y][endPoint.x] = true;
    }
  }

  std::vector<Point> floorCandidates;
  floorCandidates.reserve(grid.size() * grid[0].size());
  auto isSeedEligible = [&](const Point &p) {
    if (grid[p.y][p.x] != CellType::FLOOR) {
      return false;
    }
    if (protectedPath[p.y][p.x]) {
      return false;
    }
    int dxStart = std::abs(p.x - startPoint.x);
    int dyStart = std::abs(p.y - startPoint.y);
    int dxEnd = std::abs(p.x - endPoint.x);
    int dyEnd = std::abs(p.y - endPoint.y);
    return (dxStart > 2 || dyStart > 2) && (dxEnd > 2 || dyEnd > 2);
  };

  for (size_t y = 1; y + 1 < grid.size(); ++y) {
    for (size_t x = 1; x + 1 < grid[y].size(); ++x) {
      Point p{static_cast<int>(x), static_cast<int>(y)};
      if (isSeedEligible(p)) {
        floorCandidates.push_back(p);
      }
    }
  }

  auto canFillWater = [&](const Point &p) {
    if (!isValidPoint(p)) {
      return false;
    }
    if (grid[p.y][p.x] != CellType::FLOOR) {
      return false;
    }
    return !protectedPath[p.y][p.x];
  };

  int area = static_cast<int>(grid.size() * grid[0].size());
  int featureCount = std::max(1, area / 500);
  featureCount = std::min(featureCount, 6);
  std::uniform_int_distribution<int> riverChance(0, 99);
  int lakeMin = std::max(6, area / 400);
  int lakeMax = std::max(lakeMin + 6, area / 200);
  lakeMax = std::min(lakeMax, 40);
  std::uniform_int_distribution<int> lakeSizeDist(lakeMin, lakeMax);
  std::uniform_int_distribution<int> riverLengthDist(12, 30);
  std::uniform_int_distribution<int> dirDist(0, 3);

  std::array<Point, 4> cardinalDirs = {
      Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};

  auto addLake = [&](const Point &seed) {
    int targetSize = lakeSizeDist(rng);
    std::vector<Point> frontier;
    frontier.push_back(seed);
    grid[seed.y][seed.x] = CellType::WATER;
    int placed = 1;
    while (placed < targetSize && !frontier.empty()) {
      std::uniform_int_distribution<int> frontierDist(
          0, static_cast<int>(frontier.size()) - 1);
      int index = frontierDist(rng);
      Point current = frontier[index];
      bool expanded = false;
      std::array<Point, 4> shuffled = cardinalDirs;
      std::shuffle(shuffled.begin(), shuffled.end(), rng);
      for (const auto &dir : shuffled) {
        Point next{current.x + dir.x, current.y + dir.y};
        if (!canFillWater(next)) {
          continue;
        }
        grid[next.y][next.x] = CellType::WATER;
        frontier.push_back(next);
        placed++;
        expanded = true;
        if (placed >= targetSize) {
          break;
        }
      }
      if (!expanded) {
        frontier.erase(frontier.begin() + index);
      }
    }
  };

  auto addRiver = [&](const Point &seed) {
    int length = riverLengthDist(rng);
    Point current = seed;
    grid[current.y][current.x] = CellType::WATER;
    int currentDir = dirDist(rng);
    for (int step = 0; step < length; ++step) {
      int attempts = 0;
      bool moved = false;
      int dirIndex = currentDir;
      while (attempts < 4) {
        Point next{current.x + cardinalDirs[dirIndex].x,
                   current.y + cardinalDirs[dirIndex].y};
        if (canFillWater(next)) {
          grid[next.y][next.x] = CellType::WATER;
          current = next;
          moved = true;
          break;
        }
        dirIndex = (dirIndex + 1) % 4;
        attempts++;
      }
      if (!moved) {
        break;
      }
      if (terrainDist(rng) >= 65) {
        currentDir = dirDist(rng);
      }
    }
  };

  if (!floorCandidates.empty()) {
    for (int featureIndex = 0; featureIndex < featureCount; ++featureIndex) {
      if (floorCandidates.empty()) {
        break;
      }
      Point seed;
      int tries = 0;
      bool foundSeed = false;
      while (tries < 8 && !floorCandidates.empty()) {
        std::uniform_int_distribution<int> seedIndexDist(
            0, static_cast<int>(floorCandidates.size()) - 1);
        int index = seedIndexDist(rng);
        seed = floorCandidates[index];
        if (canFillWater(seed)) {
          foundSeed = true;
          break;
        }
        floorCandidates.erase(floorCandidates.begin() + index);
        tries++;
      }
      if (!foundSeed) {
        break;
      }
      if (riverChance(rng) < 35) {
        addRiver(seed);
      } else {
        addLake(seed);
      }
    }
  }

  auto hasPath = [&]() {
    if (!isValidPoint(startPoint) || !isValidPoint(endPoint)) {
      return false;
    }
    if (!isWalkable(startPoint) || !isWalkable(endPoint)) {
      return false;
    }
    std::queue<Point> frontier;
    std::vector<std::vector<bool>> visited(
        grid.size(), std::vector<bool>(grid[0].size(), false));
    frontier.push(startPoint);
    visited[startPoint.y][startPoint.x] = true;
    const std::array<Point, 8> directions = {
        Point{1, 0},   Point{-1, 0},  Point{0, 1},  Point{0, -1},
        Point{1, 1},   Point{-1, -1}, Point{1, -1}, Point{-1, 1}};
    while (!frontier.empty()) {
      Point current = frontier.front();
      frontier.pop();
      if (current.x == endPoint.x && current.y == endPoint.y) {
        return true;
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
    return false;
  };

  if (!hasPath()) {
    for (auto &row : grid) {
      for (auto &cell : row) {
        if (cell == CellType::WATER) {
          cell = CellType::FLOOR;
        }
      }
    }
  }

  std::vector<Point> grassCandidates;
  grassCandidates.reserve(grid.size() * grid[0].size());
  for (size_t y = 1; y + 1 < grid.size(); ++y) {
    for (size_t x = 1; x + 1 < grid[y].size(); ++x) {
      if (grid[y][x] == CellType::FLOOR) {
        grassCandidates.emplace_back(static_cast<int>(x),
                                     static_cast<int>(y));
      }
    }
  }

  int grassFeatureCount = std::max(1, area / 450);
  grassFeatureCount = std::min(grassFeatureCount, 8);
  int grassMin = std::max(5, area / 450);
  int grassMax = std::max(grassMin + 5, area / 260);
  grassMax = std::min(grassMax, 55);
  std::uniform_int_distribution<int> grassSizeDist(grassMin, grassMax);

  auto canFillGrass = [&](const Point &p) {
    if (!isValidPoint(p)) {
      return false;
    }
    return grid[p.y][p.x] == CellType::FLOOR;
  };

  auto addGrassPatch = [&](const Point &seed) {
    int targetSize = grassSizeDist(rng);
    std::vector<Point> frontier;
    frontier.push_back(seed);
    grid[seed.y][seed.x] = CellType::GRASS;
    int placed = 1;
    while (placed < targetSize && !frontier.empty()) {
      std::uniform_int_distribution<int> frontierDist(
          0, static_cast<int>(frontier.size()) - 1);
      int index = frontierDist(rng);
      Point current = frontier[index];
      bool expanded = false;
      std::array<Point, 4> shuffled = cardinalDirs;
      std::shuffle(shuffled.begin(), shuffled.end(), rng);
      for (const auto &dir : shuffled) {
        Point next{current.x + dir.x, current.y + dir.y};
        if (!canFillGrass(next)) {
          continue;
        }
        grid[next.y][next.x] = CellType::GRASS;
        frontier.push_back(next);
        placed++;
        expanded = true;
        if (placed >= targetSize) {
          break;
        }
      }
      if (!expanded) {
        frontier.erase(frontier.begin() + index);
      }
    }
  };

  if (!grassCandidates.empty()) {
    for (int featureIndex = 0; featureIndex < grassFeatureCount;
         ++featureIndex) {
      if (grassCandidates.empty()) {
        break;
      }
      std::uniform_int_distribution<int> seedIndexDist(
          0, static_cast<int>(grassCandidates.size()) - 1);
      int index = seedIndexDist(rng);
      Point seed = grassCandidates[index];
      if (canFillGrass(seed)) {
        addGrassPatch(seed);
      }
      grassCandidates.erase(grassCandidates.begin() + index);
    }
  }

  return grid;
}

unsigned int Map::getWidth() const { return width; }

unsigned int Map::getHeight() const { return height; }
