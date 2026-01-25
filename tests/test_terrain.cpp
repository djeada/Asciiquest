#include "model/map.h"
#include "utils/game_settings.h"
#include "gtest/gtest.h"

TEST(TerrainTest, MapGeneratesBasicTerrain) {
  // Arrange
  Map map(50, 50);
  
  // Act
  map.loadLevel();
  
  // Assert - Check that basic terrain types are generated (walls and floors)
  bool hasWall = false;
  bool hasFloor = false;
  bool hasDoor = false;
  
  for (unsigned int y = 0; y < map.getHeight(); ++y) {
    for (unsigned int x = 0; x < map.getWidth(); ++x) {
      Point p(x, y);
      CellType cell = map.getCellType(p);
      
      if (cell == CellType::WALL) hasWall = true;
      if (cell == CellType::FLOOR) hasFloor = true;
      if (cell == CellType::DOOR) hasDoor = true;
    }
  }
  
  // BSP dungeon should have walls, floors, and potentially doors
  EXPECT_TRUE(hasWall);
  EXPECT_TRUE(hasFloor);
}

TEST(TerrainTest, PassableTerrainAllowsMovement) {
  // Arrange
  Map map(10, 10);
  map.clear();
  
  Point grassPoint(5, 5);
  Point treePoint(6, 5);
  Point desertPoint(7, 5);
  
  // Act
  map.setCellType(grassPoint, CellType::GRASS);
  map.setCellType(treePoint, CellType::TREE);
  map.setCellType(desertPoint, CellType::DESERT);
  
  // Assert
  EXPECT_TRUE(map.isPositionFree(grassPoint));
  EXPECT_TRUE(map.isPositionFree(treePoint));
  EXPECT_TRUE(map.isPositionFree(desertPoint));
}

TEST(TerrainTest, BlockingTerrainPreventsMovement) {
  // Arrange
  Map map(10, 10);
  map.clear();
  
  Point mountainPoint(5, 5);
  Point waterPoint(6, 5);
  Point wallPoint(7, 5);
  
  // Act
  map.setCellType(mountainPoint, CellType::MOUNTAIN);
  map.setCellType(waterPoint, CellType::WATER);
  map.setCellType(wallPoint, CellType::WALL);
  
  // Assert
  EXPECT_FALSE(map.isPositionFree(mountainPoint));
  EXPECT_FALSE(map.isPositionFree(waterPoint));
  EXPECT_FALSE(map.isPositionFree(wallPoint));
}
