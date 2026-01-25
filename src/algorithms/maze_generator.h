#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

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
  BSP,
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
  void generateBSP();
  
  // BSP helper structures and methods
  struct BSPNode {
    int x, y, width, height;
    BSPNode* left = nullptr;
    BSPNode* right = nullptr;
    int roomX, roomY, roomW, roomH;
    bool hasRoom = false;
  };
  
  void splitBSP(BSPNode* node, int depth, std::default_random_engine& rng);
  void createRooms(BSPNode* node, std::default_random_engine& rng);
  void connectRooms(BSPNode* node, std::default_random_engine& rng);
  std::pair<int, int> getRoomCenter(BSPNode* node);
  std::pair<int, int> getRoomEdge(BSPNode* node, int dirX, int dirY);
  void carveHorizontalCorridor(int x1, int x2, int y);
  void carveVerticalCorridor(int y1, int y2, int x);

public:
  MazeGenerator(int width, int height, MazeGeneratorAlgorithm algorithm);
  std::vector<std::string> getMaze();
  std::pair<unsigned int, unsigned int> getStart();
  std::pair<unsigned int, unsigned int> getEnd();
};
#endif // MAZE_GENERATOR_H
