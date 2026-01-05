#include "model/model.h"
#include "utils/game_settings.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <queue>
#include <array>

namespace {
// Helper function to check if a path exists between two points
bool hasPath(const std::shared_ptr<Map> &map, const Point &start, const Point &end,
             const std::unordered_map<Point, std::shared_ptr<MovableObject>> &movableObjects,
             bool treatMovablesAsBlocking = true) {
  if (!map->isValidPoint(start) || !map->isValidPoint(end)) {
    return false;
  }
  
  auto isWalkable = [&](const Point &p) {
    if (!map->isValidPoint(p)) return false;
    CellType cell = map->getCellType(p);
    
    if (treatMovablesAsBlocking) {
      if (cell == CellType::CRATE || cell == CellType::BARREL || cell == CellType::BOULDER) {
        return false;
      }
    }
    
    return cell == CellType::FLOOR || cell == CellType::DOOR ||
           cell == CellType::GRASS || cell == CellType::TREE ||
           cell == CellType::DESERT || cell == CellType::END ||
           cell == CellType::PLAYER || cell == CellType::START ||
           cell == CellType::GOBLIN || cell == CellType::ORC ||
           cell == CellType::TROLL || cell == CellType::DRAGON ||
           cell == CellType::SKELETON || cell == CellType::TREASURE ||
           cell == CellType::POTION ||
           (!treatMovablesAsBlocking && 
            (cell == CellType::CRATE || cell == CellType::BARREL || cell == CellType::BOULDER));
  };
  
  std::queue<Point> frontier;
  std::vector<std::vector<bool>> visited(
      map->getHeight(), std::vector<bool>(map->getWidth(), false));
  
  frontier.push(start);
  visited[start.y][start.x] = true;
  
  const std::array<Point, 4> directions = {
      Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
  
  while (!frontier.empty()) {
    Point current = frontier.front();
    frontier.pop();
    
    if (current == end) {
      return true;
    }
    
    for (const auto &dir : directions) {
      Point next{current.x + dir.x, current.y + dir.y};
      if (!map->isValidPoint(next) || visited[next.y][next.x]) continue;
      if (!isWalkable(next) && next != end) continue;
      
      visited[next.y][next.x] = true;
      frontier.push(next);
    }
  }
  
  return false;
}
}  // namespace

TEST(PocketBlockingTest, ModelHasMovableObjects) {
  // Arrange & Act
  Model model;
  model.restart();
  
  // Assert - movable objects should be placed
  EXPECT_FALSE(model.movableObjects.empty());
}

TEST(PocketBlockingTest, AtLeastTwoBlockingObjects) {
  // Arrange & Act
  Model model;
  model.restart();
  
  // Assert - should have at least 2 blocking objects
  int cratesAndBarrels = 0;
  for (const auto &[pos, obj] : model.movableObjects) {
    CellType type = obj->cellType;
    if (type == CellType::CRATE || type == CellType::BARREL) {
      cratesAndBarrels++;
    }
  }
  
  EXPECT_GE(cratesAndBarrels, 2);
}

TEST(PocketBlockingTest, PathExistsAfterPushing) {
  // Arrange & Act
  Model model;
  model.restart();
  
  Point start = model.map->getStart();
  Point end = model.map->getEnd();
  
  // Assert - path should exist when treating movable objects as pushable
  bool pathExists = hasPath(model.map, start, end, model.movableObjects, false);
  EXPECT_TRUE(pathExists);
}

TEST(PocketBlockingTest, MovableObjectsOnFloorTiles) {
  // Arrange & Act
  Model model;
  model.restart();
  
  // Assert - all movable objects should have valid underlying cells
  for (const auto &[pos, obj] : model.movableObjects) {
    CellType underlying = obj->underlyingCell;
    // Underlying cell should be a walkable type (floor, grass, etc.)
    EXPECT_TRUE(underlying == CellType::FLOOR || 
                underlying == CellType::GRASS || 
                underlying == CellType::DOOR ||
                underlying == CellType::TREE ||
                underlying == CellType::DESERT);
  }
}

TEST(PocketBlockingTest, MovableObjectsArePushable) {
  // Arrange & Act
  Model model;
  model.restart();
  
  // Assert - all movable objects should be pushable
  for (const auto &[pos, obj] : model.movableObjects) {
    EXPECT_TRUE(obj->isPushable());
  }
}

TEST(PocketBlockingTest, ObjectsNotOnStartOrEnd) {
  // Arrange & Act
  Model model;
  model.restart();
  
  Point start = model.map->getStart();
  Point end = model.map->getEnd();
  
  // Assert - no movable objects should be on start or end positions
  for (const auto &[pos, obj] : model.movableObjects) {
    EXPECT_NE(pos, start);
    EXPECT_NE(pos, end);
  }
}
