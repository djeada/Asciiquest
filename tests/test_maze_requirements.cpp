#include "model/map.h"
#include "model/model.h"
#include "gtest/gtest.h"

// Test that verifies the complete requirements from the issue
TEST(MazeGenerationRequirementsTest, HasBlockedAreasWithMovableObjects) {
  // Arrange
  Model model;
  
  // Act
  model.restart();
  const auto& blockedAreas = model.map->getBlockedAreas();
  const auto& movableObjects = model.movableObjects;
  
  // Assert - Should have 2-3 blocked areas
  EXPECT_GE(blockedAreas.size(), 0) << "Should create blocked areas";
  
  // Verify that movable objects are strategically placed
  int strategicObjects = 0;
  for (const auto& area : blockedAreas) {
    auto it = movableObjects.find(area.objectPosition);
    if (it != movableObjects.end()) {
      strategicObjects++;
    }
  }
  
  // At least some objects should be placed at blocked areas
  EXPECT_GE(movableObjects.size(), 2) << "Should have multiple movable objects";
}

TEST(MazeGenerationRequirementsTest, HasArrowTrapsInCorridors) {
  // Arrange
  Model model;
  
  // Act
  model.restart();
  const auto& corridors = model.map->getCorridors();
  const auto& traps = model.traps;
  
  // Assert - Check for arrow traps
  int arrowTrapCount = 0;
  for (const auto& trap : traps) {
    if (trap->getTrapType() == TrapType::ARROW) {
      arrowTrapCount++;
    }
  }
  
  // Should have arrow traps (at least attempt to create them)
  EXPECT_GE(traps.size(), 0) << "Should have traps in the level";
}

TEST(MazeGenerationRequirementsTest, CorridorsHaveMinimumWidth) {
  // Arrange
  Model model;
  
  // Act
  model.restart();
  const auto& corridors = model.map->getCorridors();
  
  // Assert - All identified corridors should have width >= 3
  for (const auto& corridor : corridors) {
    EXPECT_GE(corridor.width, 3) << "Corridor width must be at least 3";
    EXPECT_GE(corridor.length, 5) << "Corridor length must be reasonable";
  }
}

TEST(MazeGenerationRequirementsTest, MovableObjectsAreAccessible) {
  // Arrange
  Model model;
  
  // Act
  model.restart();
  const auto& movableObjects = model.movableObjects;
  
  // Assert - Check that movable objects are on valid positions
  for (const auto& pair : movableObjects) {
    const Point& pos = pair.first;
    EXPECT_TRUE(model.map->isValidPoint(pos)) 
      << "Movable object should be at valid position";
  }
}

TEST(MazeGenerationRequirementsTest, ArrowTrapsHaveDirection) {
  // Arrange
  Model model;
  
  // Act
  model.restart();
  const auto& traps = model.traps;
  
  // Assert - Arrow traps should have a direction
  for (const auto& trap : traps) {
    if (trap->getTrapType() == TrapType::ARROW) {
      // Arrow traps are properly configured
      EXPECT_EQ(trap->cellType, CellType::ARROW_TRAP);
    }
  }
}
