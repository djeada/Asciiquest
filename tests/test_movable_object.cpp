#include "model/entities/movable_object.h"
#include "gtest/gtest.h"

TEST(MovableObjectTest, BoulderCreationAndProperties) {
  // Arrange & Act
  Point position(5, 5);
  Boulder boulder(position);

  // Assert
  EXPECT_EQ(boulder.position.x, 5);
  EXPECT_EQ(boulder.position.y, 5);
  EXPECT_EQ(boulder.cellType, CellType::BOULDER);
  EXPECT_TRUE(boulder.isPushable());
  EXPECT_EQ(boulder.getWeight(), 3);
  EXPECT_EQ(boulder.toString(), "Boulder");
}

TEST(MovableObjectTest, CrateCreationAndProperties) {
  // Arrange & Act
  Point position(3, 4);
  Crate crate(position);

  // Assert
  EXPECT_EQ(crate.position.x, 3);
  EXPECT_EQ(crate.position.y, 4);
  EXPECT_EQ(crate.cellType, CellType::CRATE);
  EXPECT_TRUE(crate.isPushable());
  EXPECT_EQ(crate.getWeight(), 1);
  EXPECT_EQ(crate.toString(), "Crate");
}

TEST(MovableObjectTest, BarrelCreationAndProperties) {
  // Arrange & Act
  Point position(7, 2);
  Barrel barrel(position);

  // Assert
  EXPECT_EQ(barrel.position.x, 7);
  EXPECT_EQ(barrel.position.y, 2);
  EXPECT_EQ(barrel.cellType, CellType::BARREL);
  EXPECT_TRUE(barrel.isPushable());
  EXPECT_EQ(barrel.getWeight(), 2);
  EXPECT_EQ(barrel.toString(), "Barrel");
}

TEST(MovableObjectTest, MoveObject) {
  // Arrange
  Point initialPos(1, 1);
  Point newPos(2, 2);
  Boulder boulder(initialPos);

  // Act
  boulder.move(newPos);

  // Assert
  EXPECT_EQ(boulder.position.x, 2);
  EXPECT_EQ(boulder.position.y, 2);
}

TEST(MovableObjectTest, WeightComparison) {
  // Arrange
  Crate crate(Point(0, 0));
  Barrel barrel(Point(0, 0));
  Boulder boulder(Point(0, 0));

  // Assert - weights should be in correct order
  EXPECT_LT(crate.getWeight(), barrel.getWeight());
  EXPECT_LT(barrel.getWeight(), boulder.getWeight());
}
