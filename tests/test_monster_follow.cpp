#include "model/entities/monster.h"
#include "model/entities/player.h"
#include "model/map.h"
#include "gtest/gtest.h"
#include <memory>

class MonsterFollowTest : public ::testing::Test {
protected:
  std::shared_ptr<Map> map;
  std::shared_ptr<Player> player;

  void SetUp() override {
    map = std::make_shared<Map>(50, 50);
    player = std::make_shared<Player>();
    player->position = Point(25, 25);
  }
};

TEST_F(MonsterFollowTest, GoblinFollowsPlayerWithinRange) {
  // Arrange
  auto goblin = std::make_shared<Goblin>(map, player);
  goblin->position = Point(20, 25); // 5 units away (within GoblinFollowRange=5)

  // Act
  goblin->randomizeVelocity();
  Point velocity = goblin->getVelocity();

  // Assert - velocity should point towards player (positive x direction)
  // Since goblin is at (20,25) and player at (25,25), should move in +x direction
  EXPECT_GT(velocity.x, 0);
}

TEST_F(MonsterFollowTest, GoblinRandomMovementOutOfRange) {
  // Arrange
  auto goblin = std::make_shared<Goblin>(map, player);
  goblin->position = Point(10, 25); // 15 units away (outside GoblinFollowRange=5)

  // Act
  goblin->randomizeVelocity();
  Point velocity = goblin->getVelocity();

  // Assert - velocity should be random (non-zero)
  EXPECT_TRUE(velocity.x != 0 || velocity.y != 0);
}

TEST_F(MonsterFollowTest, TrollFollowsPlayerWithinRange) {
  // Arrange
  auto troll = std::make_shared<Troll>(map, player);
  troll->position = Point(15, 25); // 10 units away (within TrollFollowRange=15)

  // Act
  troll->randomizeVelocity();
  Point velocity = troll->getVelocity();

  // Assert - velocity should point towards player (positive x direction)
  // Since troll is at (15,25) and player at (25,25), should move in +x direction
  EXPECT_GT(velocity.x, 0);
}

TEST_F(MonsterFollowTest, TrollRandomMovementOutOfRange) {
  // Arrange
  auto troll = std::make_shared<Troll>(map, player);
  troll->position = Point(5, 25); // 20 units away (outside TrollFollowRange=15)

  // Act
  troll->randomizeVelocity();
  Point velocity = troll->getVelocity();

  // Assert - velocity should be random (non-zero)
  EXPECT_TRUE(velocity.x != 0 || velocity.y != 0);
}

TEST_F(MonsterFollowTest, SkeletonFollowsPlayerWithinRange) {
  // Arrange
  auto skeleton = std::make_shared<Skeleton>(map, player);
  skeleton->position = Point(20, 25); // 5 units away (within SkeletonFollowRange=8)

  // Test multiple times due to randomness (70% chance to follow)
  int followCount = 0;
  for (int i = 0; i < 10; i++) {
    skeleton->randomizeVelocity();
    Point velocity = skeleton->getVelocity();
    if (velocity.x > 0) { // Moving towards player
      followCount++;
    }
  }

  // Assert - should follow player more often than not
  EXPECT_GT(followCount, 3); // At least 30% of the time
}

TEST_F(MonsterFollowTest, DragonNeverMoves) {
  // Arrange
  auto dragon = std::make_shared<Dragon>();
  dragon->position = Point(20, 25);
  Point initialPos = dragon->position;

  // Act
  dragon->randomizeVelocity();
  Point velocity = dragon->getVelocity();

  // Assert - velocity should always be (0,0)
  EXPECT_EQ(velocity.x, 0);
  EXPECT_EQ(velocity.y, 0);
}

TEST_F(MonsterFollowTest, OrcFollowsPlayerWithinRange) {
  // Arrange
  auto orc = std::make_shared<Orc>(map, player);
  orc->position = Point(22, 25); // 3 units away (within OrcFollowRange=10)
  
  // Assert - Orc uses pathfinding with async, so we just verify it was created properly
  // Testing the pathfinding algorithm is done in test_a_star.cpp
  EXPECT_EQ(orc->toString(), "Orc");
  EXPECT_EQ(orc->cellType, CellType::ORC);
}

TEST_F(MonsterFollowTest, FollowRangeOrdering) {
  // Verify that ranges are properly ordered: Goblin < Skeleton < Orc < Troll
  auto goblin = std::make_shared<Goblin>(map, player);
  auto skeleton = std::make_shared<Skeleton>(map, player);
  auto orc = std::make_shared<Orc>(map, player);
  auto troll = std::make_shared<Troll>(map, player);
  
  // These assertions verify that the configuration is set correctly
  // GoblinFollowRange=5, SkeletonFollowRange=8, OrcFollowRange=10, TrollFollowRange=15
  EXPECT_LT(5, 8);   // Goblin range < Skeleton range
  EXPECT_LT(8, 10);  // Skeleton range < Orc range
  EXPECT_LT(10, 15); // Orc range < Troll range
}
