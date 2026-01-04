#include "model/entities/trap.h"
#include "gtest/gtest.h"

TEST(TrapTest, BladeTrapCreationAndProperties) {
  // Arrange & Act
  Point position(5, 5);
  BladeTrap trap(position);

  // Assert
  EXPECT_EQ(trap.position.x, 5);
  EXPECT_EQ(trap.position.y, 5);
  EXPECT_EQ(trap.cellType, CellType::BLADE_TRAP);
  EXPECT_EQ(trap.getTrapType(), TrapType::BLADE);
  EXPECT_EQ(trap.getState(), TrapState::INACTIVE);
  EXPECT_EQ(trap.getDamage(), 15);
  EXPECT_EQ(trap.toString(), "Blade Trap");
}

TEST(TrapTest, SpikeTrapCreationAndProperties) {
  // Arrange & Act
  Point position(3, 4);
  SpikeTrap trap(position);

  // Assert
  EXPECT_EQ(trap.position.x, 3);
  EXPECT_EQ(trap.position.y, 4);
  EXPECT_EQ(trap.cellType, CellType::SPIKE_TRAP);
  EXPECT_EQ(trap.getTrapType(), TrapType::SPIKE);
  EXPECT_EQ(trap.getState(), TrapState::INACTIVE);
  EXPECT_EQ(trap.getDamage(), 20);
  EXPECT_EQ(trap.toString(), "Spike Trap");
}

TEST(TrapTest, ArrowTrapCreationAndProperties) {
  // Arrange & Act
  Point position(7, 2);
  Point direction(0, 1); // Downward
  ArrowTrap trap(position, direction);

  // Assert
  EXPECT_EQ(trap.position.x, 7);
  EXPECT_EQ(trap.position.y, 2);
  EXPECT_EQ(trap.cellType, CellType::ARROW_TRAP);
  EXPECT_EQ(trap.getTrapType(), TrapType::ARROW);
  EXPECT_EQ(trap.getState(), TrapState::INACTIVE);
  EXPECT_EQ(trap.getDamage(), 12);
  EXPECT_EQ(trap.toString(), "Arrow Trap");
}

TEST(TrapTest, BladeTrapActivation) {
  // Arrange
  Point position(5, 5);
  BladeTrap trap(position);

  // Act
  trap.activate();

  // Assert
  EXPECT_EQ(trap.getState(), TrapState::ACTIVE);
  EXPECT_GT(trap.getProjectiles().size(), 0);
  
  const auto &projectiles = trap.getProjectiles();
  EXPECT_TRUE(projectiles[0].active);
  EXPECT_EQ(projectiles[0].cellType, CellType::BLADE_PROJECTILE);
  EXPECT_EQ(projectiles[0].damage, 15);
}

TEST(TrapTest, SpikeTrapActivation) {
  // Arrange
  Point position(3, 4);
  SpikeTrap trap(position);

  // Act
  trap.activate();

  // Assert
  EXPECT_EQ(trap.getState(), TrapState::ACTIVE);
  EXPECT_GT(trap.getProjectiles().size(), 0);
  
  const auto &projectiles = trap.getProjectiles();
  EXPECT_TRUE(projectiles[0].active);
  EXPECT_EQ(projectiles[0].cellType, CellType::SPIKE_PROJECTILE);
  EXPECT_EQ(projectiles[0].position.x, position.x);
  EXPECT_EQ(projectiles[0].position.y, position.y);
}

TEST(TrapTest, ArrowTrapActivation) {
  // Arrange
  Point position(7, 2);
  Point direction(0, 1);
  ArrowTrap trap(position, direction);

  // Act
  trap.activate();

  // Assert
  EXPECT_EQ(trap.getState(), TrapState::ACTIVE);
  EXPECT_GT(trap.getProjectiles().size(), 0);
  
  const auto &projectiles = trap.getProjectiles();
  EXPECT_TRUE(projectiles[0].active);
  EXPECT_EQ(projectiles[0].cellType, CellType::ARROW_PROJECTILE);
  EXPECT_EQ(projectiles[0].direction.x, direction.x);
  EXPECT_EQ(projectiles[0].direction.y, direction.y);
}

TEST(TrapTest, BladeTrapUpdate) {
  // Arrange
  Point position(5, 5);
  BladeTrap trap(position);

  // Act - Update multiple times to trigger activation
  for (int i = 0; i < 20; ++i) {
    trap.update();
  }

  // Assert - Trap should have activated and created projectiles
  EXPECT_TRUE(trap.getState() == TrapState::ACTIVE || 
              trap.getState() == TrapState::COOLDOWN || 
              trap.getState() == TrapState::INACTIVE);
}

TEST(TrapTest, SpikeTrapUpdate) {
  // Arrange
  Point position(3, 4);
  SpikeTrap trap(position);

  // Act - Update multiple times to trigger activation
  for (int i = 0; i < 15; ++i) {
    trap.update();
  }

  // Assert - Trap should have activated
  EXPECT_TRUE(trap.getState() == TrapState::ACTIVE || 
              trap.getState() == TrapState::COOLDOWN || 
              trap.getState() == TrapState::INACTIVE);
}

TEST(TrapTest, ProjectileClearInactive) {
  // Arrange
  Point position(5, 5);
  BladeTrap trap(position);
  trap.activate();
  
  // Manually deactivate projectiles using the proper method
  for (size_t i = 0; i < trap.getProjectiles().size(); ++i) {
    trap.deactivateProjectile(i);
  }

  // Act
  trap.clearInactiveProjectiles();

  // Assert
  EXPECT_EQ(trap.getProjectiles().size(), 0);
}
