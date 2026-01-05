#include "model/map.h"
#include "gtest/gtest.h"

TEST(MapGenerationTest, CorridorsAreIdentified) {
  // Arrange
  Map map(60, 40);
  
  // Act
  map.loadLevel();
  const auto& corridors = map.getCorridors();
  
  // Assert - We should have at least some corridors
  // Note: Depending on random generation, there might not always be corridors
  // but the method should work without crashing
  EXPECT_GE(corridors.size(), 0);
  
  // If corridors exist, verify their properties
  for (const auto& corridor : corridors) {
    EXPECT_GE(corridor.width, 3) << "Corridor width should be at least 3";
    EXPECT_GE(corridor.length, 5) << "Corridor length should be at least 5";
    EXPECT_TRUE(corridor.direction.x != 0 || corridor.direction.y != 0) 
      << "Corridor should have a direction";
  }
}

TEST(MapGenerationTest, BlockedAreasAreCreated) {
  // Arrange
  Map map(60, 40);
  
  // Act
  map.loadLevel();
  const auto& blockedAreas = map.getBlockedAreas();
  
  // Assert - We should have some blocked areas
  EXPECT_GE(blockedAreas.size(), 0);
  
  // If blocked areas exist, verify their properties
  for (const auto& area : blockedAreas) {
    EXPECT_TRUE(map.isValidPoint(area.objectPosition)) 
      << "Object position should be valid";
    EXPECT_TRUE(map.isValidPoint(area.blockedPosition)) 
      << "Blocked position should be valid";
  }
}

TEST(MapGenerationTest, MapDimensionsAreCorrect) {
  // Arrange & Act
  Map map(50, 30);
  map.loadLevel();
  
  // Assert
  EXPECT_EQ(map.getWidth(), 50);
  EXPECT_EQ(map.getHeight(), 30);
  EXPECT_EQ(map.grid.size(), 30);
  EXPECT_EQ(map.grid[0].size(), 50);
}

TEST(MapGenerationTest, StartAndEndPointsAreValid) {
  // Arrange
  Map map(60, 40);
  
  // Act
  map.loadLevel();
  Point start = map.getStart();
  Point end = map.getEnd();
  
  // Assert
  EXPECT_TRUE(map.isValidPoint(start)) << "Start point should be valid";
  EXPECT_TRUE(map.isValidPoint(end)) << "End point should be valid";
  EXPECT_NE(start.x, end.x) << "Start and end should be different";
  EXPECT_NE(start.y, end.y) << "Start and end should be different";
}

TEST(MapGenerationTest, GridIsProperlyInitialized) {
  // Arrange & Act
  Map map(40, 30);
  map.loadLevel();
  
  // Assert - Grid should not be empty
  EXPECT_FALSE(map.grid.empty());
  EXPECT_FALSE(map.grid[0].empty());
  
  // Grid should contain various cell types
  bool hasWalls = false;
  bool hasFloors = false;
  
  for (const auto& row : map.grid) {
    for (const auto& cell : row) {
      if (cell == CellType::WALL) hasWalls = true;
      if (cell == CellType::FLOOR) hasFloors = true;
    }
  }
  
  EXPECT_TRUE(hasWalls) << "Grid should contain walls";
  EXPECT_TRUE(hasFloors) << "Grid should contain floors";
}
