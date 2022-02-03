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

        std::vector<std::pair<unsigned int, unsigned int>> getNeighbors(unsigned int x, unsigned int y) {
            /**
             * @brief Returns the neighbors of a cell.
             * @param x The x coordinate of the cell.
             * @param y The y coordinate of the cell.
             * @return A vector of the neighbors of the cell.
             */
            std::vector<std::pair<unsigned int, unsigned int>> neighbors;

            // add cell right of current cell if it exists
            if (x > 1) {
                neighbors.push_back(std::make_pair(x - 1,y));
            }
            // add cell left of current cell if it exists
            if (x < width - 2) {
                neighbors.push_back(std::make_pair(x + 1, y));
            }
            // add cell above current cell if it exists
            if (y > 1) {
                neighbors.push_back(std::make_pair(x, y - 1));
            }
            // add cell below current cell if it exists
            if (y < height - 2) {
                neighbors.push_back(std::make_pair(x, y + 1));
            }
            return neighbors;
        }    
    
        void generateRecursiveDFS() {
            /**
             * @brief Generates a maze using the recursive backtracking algorithm.
             */
            std::pair<unsigned int, unsigned int> current = this->start;
            std::vector<std::pair<unsigned int, unsigned int>> stack = {current};
            std::default_random_engine random_engine (time(0));
            while (stack.size() > 0) {
                current = stack.back();
                if (current == this->end)
                    break;
                stack.pop_back();
                this->maze[current.second][current.first] = ' ';
                auto neighbors = this->getNeighbors(current.first, current.second);
                std::shuffle(neighbors.begin(), neighbors.end(), random_engine);
                for (auto neighbor : neighbors) {
                    if (this->maze[neighbor.second][neighbor.first] == '#') {
                        stack.push_back(neighbor);
                    }
                }
            }
        }

        void generateRandomizedPrim() {
            /**
             * @brief Generates a maze using the randomized prims algorithm.
             */
            std::priority_queue<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>> queue;
            std::pair<unsigned int, unsigned int> current = this->start;
            queue.push(std::make_pair(0, current));
            std::default_random_engine random_engine (time(0));
            std::uniform_int_distribution<size_t> distribution(0, 100);
            while (!queue.empty()) {
                auto currentDistance = queue.top().first;
                current = queue.top().second;
                if (current == this->end)
                    break;
                queue.pop();
                this->maze[current.second][current.first] = ' ';
                auto neighbors = this->getNeighbors(current.first, current.second);
                for (auto neighbor : neighbors) {
                    if (this->maze[neighbor.second][neighbor.first] == '#') {
                        auto distance = distribution(random_engine);
                        queue.push(std::make_pair(currentDistance + distance, neighbor));
                    }
                }
            }
        }

    public: 
        MazeGenerator(int width, int height, MazeGeneratorAlgorithm algorithm) {
            this->width = width;
            this->height = height;
            this->algorithm = algorithm;
            this->maze = std::vector<std::string>(height, std::string(width, '#'));
            this->start = std::make_pair(1, 1);
            this->end = std::make_pair(width - 2, height - 2);
            switch (algorithm) {
                case MazeGeneratorAlgorithm::DepthFirstSearch:
                    this->generateRecursiveDFS();
                    break;
                case MazeGeneratorAlgorithm::RandomizedPrim:
                    this->generateRandomizedPrim();
                    break;
                default:
                    // throw exception not implemented
                    throw "Not implemented";
            }
        }

        std::vector<std::string> getMaze() {
            /**
             * @brief Returns the maze.
             * @return The maze.
             */
            return this->maze;
        }

        std::pair<unsigned int, unsigned int> getStart() {
            /**
             * @brief Returns the start position.
             * @return The start position.
             */
            return this->start;
        }

        std::pair<unsigned int, unsigned int> getEnd() {
            /**
             * @brief Returns the end position.
             * @return The end position.
             */
            return this->end;
        }

};

int main() {
    MazeGenerator mazeGenerator(50, 20, MazeGeneratorAlgorithm::RandomizedPrim);
    auto maze = mazeGenerator.getMaze();
    auto start = mazeGenerator.getStart();
    auto end = mazeGenerator.getEnd();
    for (auto row : maze) {
        for (auto cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << "Start: " << start.first << ", " << start.second << std::endl;
    std::cout << "End: " << end.first << ", " << end.second << std::endl;
    return 0;
}

        
    
