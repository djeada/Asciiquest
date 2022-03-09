// Use gtest to test the functions in maze_generator.h
//
// Language: cpp
// Path: cpp\test_maze_generator.cpp
// Compare this snippet from cpp\maze_generator.cpp:

#include "gtest/gtest.h"
#inlcude "maze_generator.h"

TEST(MazeGenerator, getNeighbors) {
  MazeGenerator mazeGenerator(10, 10);
  std::pair<unsigned int, unsigned int> current = std::make_pair(1, 1);
  std::vector<std::pair<unsigned int, unsigned int>> expected = {
      std::make_pair(2, 1),
      std::make_pair(1, 2),
  };

  auto neighbors = mazeGenerator.getNeighbors(current.first, current.second);

  std::sort(neighbors.begin(), neighbors.end());
  std::sort(expected.begin(), expected.end());

  ASSERT_EQ(neighbors.size(), expected.size());

  for (unsigned int i = 0; i < neighbors.size(); i++) {
    ASSERT_EQ(neighbors[i], expected[i]);
  }
}

TEST(MazeGenerator, generateRecursiveDFS) {
  MazeGenerator mazeGenerator(10, 10);
  auto originalMaze = mazeGenerator.getMaze();
  mazeGenerator.generateRecursiveDFS();
  auto generatedMaze = mazeGenerator.getMaze();

  ASSERT_EQ(originalMaze.size(), generatedMaze.size());
  ASSERT_NE(originalMaze, generatedMaze);
  ASSERT_CONTAINS(generatedMaze, ' ');
  ASSERT_CONTAINS(generatedMaze, '#');
}

TEST(MazeGenerator, generateRandomizedPrim) {
  MazeGenerator mazeGenerator(10, 10);
  auto originalMaze = mazeGenerator.getMaze();
  mazeGenerator.generateRandomizedPrim();
  auto generatedMaze = mazeGenerator.getMaze();

  ASSERT_EQ(originalMaze.size(), generatedMaze.size());
  ASSERT_NE(originalMaze, generatedMaze);
  ASSERT_CONTAINS(generatedMaze, ' ');
  ASSERT_CONTAINS(generatedMaze, '#');
}
