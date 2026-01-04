#include "model/map.h"
#include "utils/game_settings.h"
#include "gtest/gtest.h"

TEST(TerrainTest, MapGeneratesTerrainTypes) {
  // Arrange
  Map map(50, 50);
  
  // Act
  map.loadLevel();
  
  // Assert - Check that terrain types are generated
  bool hasGrass = false;
  bool hasTree = false;
  bool hasWater = false;
  bool hasDesert = false;
  bool hasMountain = false;
  
  for (unsigned int y = 0; y < map.getHeight(); ++y) {
    for (unsigned int x = 0; x < map.getWidth(); ++x) {
      Point p(x, y);
      CellType cell = map.getCellType(p);
      
      if (cell == CellType::GRASS) hasGrass = true;
      if (cell == CellType::TREE) hasTree = true;
      if (cell == CellType::WATER) hasWater = true;
      if (cell == CellType::DESERT) hasDesert = true;
      if (cell == CellType::MOUNTAIN) hasMountain = true;
    }
  }
  
  // At least some terrain types should be present in a 50x50 map
  EXPECT_TRUE(hasGrass || hasTree || hasWater || hasDesert || hasMountain);
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
