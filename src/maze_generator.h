#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MAZE_GENERATOR_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_MAZE_GENERATOR_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <utility>
#include <vector>

enum class MazeGeneratorAlgorithm {
  DepthFirstSearch,
  RandomizedPrim,
  RecursiveBacktracker,
  AldousBroder,
  Wilsons,
  HuntAndKill,
  BinaryTree,
  Sidewinder,
  AldousBroderHexagonal,
  BinaryTreeHexagonal,
  SidewinderHexagonal,
  RecursiveBacktrackerHexagonal,
  RandomizedPrimsHexagonal,
  HuntAndKillHexagonal,
  WilsonsHexagonal,
  DepthFirstSearchHexagonal,
  Unknown
};

class MazeGenerator {
  /**
   * @brief Generates a maze using the recursive backtracking algorithm.
   * @param width The width of the maze.
   * @param height The height of the maze.
   * @param algorithm The algorithm to use.
   */
private:
  unsigned int width;
  unsigned int height;
  MazeGeneratorAlgorithm algorithm;
  std::vector<std::string> maze;
  std::pair<unsigned int, unsigned int> start;
  std::pair<unsigned int, unsigned int> end;

  std::vector<std::pair<unsigned int, unsigned int>>
  getNeighbors(unsigned int x, unsigned int y) const;
  void generateRecursiveDFS();
  void generateRandomizedPrim();

public:
  MazeGenerator(int width, int height, MazeGeneratorAlgorithm algorithm);
  std::vector<std::string> getMaze();
  std::pair<unsigned int, unsigned int> getStart();
  std::pair<unsigned int, unsigned int> getEnd();
};
#endif
