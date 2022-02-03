#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <random>
#include <queue>
#include <stack>
#include <iostream>

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

class MazeGenerator
{
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

        std::vector<std::pair<unsigned int, unsigned int>> getNeighbors(unsigned int x, unsigned int y);
        void generateRecursiveDFS();
        void generateRandomizedPrim();

    public: 
        MazeGenerator(int width, int height, MazeGeneratorAlgorithm algorithm);
        std::vector<std::string> getMaze();
        std::pair<unsigned int, unsigned int> getStart();
        std::pair<unsigned int, unsigned int> getEnd();

};