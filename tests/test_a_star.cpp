#include "algorithms/a_star.h"
#include "utils/game_settings.h"
#include "gtest/gtest.h"
#include <climits>
#include <cmath>

TEST(AStarTest, FindsOnlyPossiblePathInGrid) {
  // Arrange
  std::vector<std::vector<int>> grid = {
      {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1}};

  Point start(0, 0);
  Point end(4, 2);

  AStar<int> aStar(grid, start, end);

  std::deque<Point> expectedPath = {Point(0, 0), Point(0, 1), Point(0, 2),
                                    Point(1, 2), Point(2, 2), Point(3, 2),
                                    Point(4, 2)};

  // Act
  std::deque<Point> actualPath = aStar.getPath();

  // Assert
  ASSERT_EQ(actualPath, expectedPath);
}

TEST(AStarTest, FindsPathInCellTypeEnumGrid) {
  // Arrange
  std::vector<std::vector<CellType>> grid = {
      {CellType::START, CellType::EMPTY, CellType::EMPTY, CellType::EMPTY,
       CellType::WALL},
      {CellType::WALL, CellType::WALL, CellType::EMPTY, CellType::WALL,
       CellType::EMPTY},
      {CellType::EMPTY, CellType::EMPTY, CellType::EMPTY, CellType::EMPTY,
       CellType::END}};

  Point start(0, 0);
  Point end(4, 2);

auto isNavigable = [](const CellType &cell) {
  return cell != CellType::WALL;
};

  AStar<CellType> aStar(grid, start, end, isNavigable);

  std::deque<Point> expectedPath = {Point(0, 0), Point(1, 0), Point(2, 0),
                                    Point(2, 1), Point(2, 2), Point(3, 2),
                                    Point(4, 2)};

  // Act
  std::deque<Point> actualPath = aStar.getPath();

  // Assert
  ASSERT_EQ(actualPath, expectedPath);
}
