#include "maze_generator.h"
#include <functional>

auto MazeGenerator::getNeighbors(unsigned int x, unsigned int y) const
    -> std::vector<std::pair<unsigned int, unsigned int>> {
  /**
   * @brief Returns the neighbors of a cell.
   * @param x The x coordinate of the cell.
   * @param y The y coordinate of the cell.
   * @return A vector of the neighbors of the cell.
   */
  std::vector<std::pair<unsigned int, unsigned int>> neighbors;

  // add cell right of current cell if it exists
  if (x > 1) {
    neighbors.push_back(std::make_pair(x - 1, y));
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

void MazeGenerator::generateRecursiveDFS() {
  /**
   * @brief Generates a maze using the recursive backtracking algorithm.
   * @return Nothing.
   */
  std::pair<unsigned int, unsigned int> current = this->start;
  std::vector<std::pair<unsigned int, unsigned int>> stack = {current};
  std::default_random_engine random_engine(time(0));
  while (!stack.empty()) {
    current = stack.back();
    if (current == this->end) {
      {
        { break; }
      }
    }
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
  // mark end with ' '
  this->maze[end.second][end.first] = ' ';
}

void MazeGenerator::generateRandomizedPrim() {
  /**
   * @brief Generates a maze using the randomized prims algorithm.
   * @return Nothing.
   */
  std::priority_queue<
      std::pair<unsigned int, std::pair<unsigned int, unsigned int>>>
      queue;
  std::pair<unsigned int, unsigned int> current = this->start;
  queue.push(std::make_pair(0, current));
  std::default_random_engine random_engine(time(0));
  std::uniform_int_distribution<size_t> distribution(0, 100);
  while (!queue.empty()) {
    auto currentDistance = queue.top().first;
    current = queue.top().second;
    if (current == this->end) {
      {
        { break; }
      }
    }
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
  // mark end with ' '
  this->maze[end.second][end.first] = ' ';
}

MazeGenerator::MazeGenerator(int width, int height,
                             MazeGeneratorAlgorithm algorithm) {
  /**
   * @brief Constructs a new MazeGenerator object.
   * @param width The width of the maze.
   * @param height The height of the maze.
   * @param algorithm The algorithm to use to generate the maze.
   * @return MazeGenerator object.
   */
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
  case MazeGeneratorAlgorithm::BSP:
    this->generateBSP();
    break;
  default:
    // throw exception not implemented
    throw "Not implemented";
  }
}

auto MazeGenerator::getMaze() -> std::vector<std::string> {
  /**
   * @brief Returns the maze.
   * @return The maze.
   */
  return this->maze;
}

auto MazeGenerator::getStart() -> std::pair<unsigned int, unsigned int> {
  /**
   * @brief Returns the start position.
   * @return The start position.
   */
  return this->start;
}

auto MazeGenerator::getEnd() -> std::pair<unsigned int, unsigned int> {
  /**
   * @brief Returns the end position.
   * @return The end position.
   */
  return this->end;
}

void MazeGenerator::generateBSP() {
  // BSP dungeon generation - creates distinct rooms connected by narrow corridors
  std::default_random_engine rng(time(0));
  
  // Create root node covering entire dungeon (minus border)
  BSPNode* root = new BSPNode();
  root->x = 1;
  root->y = 1;
  root->width = width - 2;
  root->height = height - 2;
  
  // Recursively split the space
  splitBSP(root, 0, rng);
  
  // Create rooms in leaf nodes
  createRooms(root, rng);
  
  // Connect rooms with narrow corridors
  connectRooms(root, rng);
  
  // Collect all rooms
  std::vector<BSPNode*> leaves;
  std::queue<BSPNode*> queue;
  queue.push(root);
  while (!queue.empty()) {
    BSPNode* node = queue.front();
    queue.pop();
    if (node->hasRoom) {
      leaves.push_back(node);
    }
    if (node->left) queue.push(node->left);
    if (node->right) queue.push(node->right);
  }
  
  if (!leaves.empty()) {
    // Start in top-left-most room
    BSPNode* startRoom = leaves.front();
    for (auto* leaf : leaves) {
      if (leaf->roomX + leaf->roomY < startRoom->roomX + startRoom->roomY) {
        startRoom = leaf;
      }
    }
    this->start = {startRoom->roomX + 1, startRoom->roomY + 1};
    
    // End in bottom-right-most room
    BSPNode* endRoom = leaves.back();
    for (auto* leaf : leaves) {
      if (leaf->roomX + leaf->roomY > endRoom->roomX + endRoom->roomY) {
        endRoom = leaf;
      }
    }
    this->end = {endRoom->roomX + endRoom->roomW - 2,
                 endRoom->roomY + endRoom->roomH - 2};
  }
  
  // Mark start and end
  this->maze[start.second][start.first] = ' ';
  this->maze[end.second][end.first] = ' ';
  
  // Clean up BSP tree
  std::function<void(BSPNode*)> cleanup = [&](BSPNode* node) {
    if (!node) return;
    cleanup(node->left);
    cleanup(node->right);
    delete node;
  };
  cleanup(root);
}

void MazeGenerator::splitBSP(BSPNode* node, int depth, std::default_random_engine& rng) {
  const int MIN_SIZE = 25;  // Larger minimum partition for bigger rooms
  const int MAX_DEPTH = 7;  // More splits = more rooms
  
  if (depth >= MAX_DEPTH) return;
  if (node->width < MIN_SIZE * 2 && node->height < MIN_SIZE * 2) return;
  
  // Decide split direction based on aspect ratio
  bool splitHorizontal;
  if (node->width > node->height * 1.25) {
    splitHorizontal = false; // Split vertically (left/right)
  } else if (node->height > node->width * 1.25) {
    splitHorizontal = true;  // Split horizontally (top/bottom)
  } else {
    std::uniform_int_distribution<int> coinFlip(0, 1);
    splitHorizontal = coinFlip(rng) == 0;
  }
  
  int max = (splitHorizontal ? node->height : node->width) - MIN_SIZE;
  if (max <= MIN_SIZE) return;
  
  std::uniform_int_distribution<int> splitDist(MIN_SIZE, max);
  int split = splitDist(rng);
  
  node->left = new BSPNode();
  node->right = new BSPNode();
  
  if (splitHorizontal) {
    node->left->x = node->x;
    node->left->y = node->y;
    node->left->width = node->width;
    node->left->height = split;
    
    node->right->x = node->x;
    node->right->y = node->y + split;
    node->right->width = node->width;
    node->right->height = node->height - split;
  } else {
    node->left->x = node->x;
    node->left->y = node->y;
    node->left->width = split;
    node->left->height = node->height;
    
    node->right->x = node->x + split;
    node->right->y = node->y;
    node->right->width = node->width - split;
    node->right->height = node->height;
  }
  
  splitBSP(node->left, depth + 1, rng);
  splitBSP(node->right, depth + 1, rng);
}

void MazeGenerator::createRooms(BSPNode* node, std::default_random_engine& rng) {
  if (!node) return;
  
  if (node->left || node->right) {
    // Not a leaf, recurse
    createRooms(node->left, rng);
    createRooms(node->right, rng);
    return;
  }
  
  // Leaf node - create a room with walls
  const int MIN_ROOM = 20;  // Bigger minimum room size
  const int PADDING = 2;    // Space between room and partition edge
  
  int maxW = node->width - PADDING * 2;
  int maxH = node->height - PADDING * 2;
  
  if (maxW < MIN_ROOM || maxH < MIN_ROOM) return;
  
  // Room size - allow larger rooms (fill most of partition)
  std::uniform_int_distribution<int> wDist(MIN_ROOM, std::min(maxW, 48));
  std::uniform_int_distribution<int> hDist(MIN_ROOM, std::min(maxH, 36));
  
  node->roomW = wDist(rng);
  node->roomH = hDist(rng);
  
  // Position room within partition
  int xRange = node->width - node->roomW - PADDING;
  int yRange = node->height - node->roomH - PADDING;
  
  std::uniform_int_distribution<int> xDist(0, std::max(0, xRange));
  std::uniform_int_distribution<int> yDist(0, std::max(0, yRange));
  
  node->roomX = node->x + PADDING + xDist(rng);
  node->roomY = node->y + PADDING + yDist(rng);
  node->hasRoom = true;
  
  // Carve room interior (leave walls as '#')
  for (int y = node->roomY + 1; y < node->roomY + node->roomH - 1; ++y) {
    for (int x = node->roomX + 1; x < node->roomX + node->roomW - 1; ++x) {
      if (y >= 0 && y < static_cast<int>(height) && 
          x >= 0 && x < static_cast<int>(width)) {
        this->maze[y][x] = ' ';
      }
    }
  }
}

std::pair<int, int> MazeGenerator::getRoomCenter(BSPNode* node) {
  if (!node) return {-1, -1};
  
  if (node->hasRoom) {
    // Return interior center (accounting for walls)
    return {node->roomX + node->roomW / 2, node->roomY + node->roomH / 2};
  }
  
  // Get center from children
  if (node->left) {
    auto center = getRoomCenter(node->left);
    if (center.first >= 0) return center;
  }
  if (node->right) {
    auto center = getRoomCenter(node->right);
    if (center.first >= 0) return center;
  }
  
  return {-1, -1};
}

std::pair<int, int> MazeGenerator::getRoomEdge(BSPNode* node, int dirX, int dirY) {
  // Get a point on the room edge in the given direction (for corridor connection)
  if (!node) return {-1, -1};
  
  if (node->hasRoom) {
    int cx = node->roomX + node->roomW / 2;
    int cy = node->roomY + node->roomH / 2;
    
    if (dirX > 0) return {node->roomX + node->roomW - 1, cy}; // Right edge
    if (dirX < 0) return {node->roomX, cy};                    // Left edge
    if (dirY > 0) return {cx, node->roomY + node->roomH - 1}; // Bottom edge
    if (dirY < 0) return {cx, node->roomY};                    // Top edge
    return {cx, cy};
  }
  
  // Get from children
  if (node->left) {
    auto edge = getRoomEdge(node->left, dirX, dirY);
    if (edge.first >= 0) return edge;
  }
  if (node->right) {
    auto edge = getRoomEdge(node->right, dirX, dirY);
    if (edge.first >= 0) return edge;
  }
  
  return {-1, -1};
}

void MazeGenerator::carveHorizontalCorridor(int x1, int x2, int y) {
  int minX = std::min(x1, x2);
  int maxX = std::max(x1, x2);
  for (int x = minX; x <= maxX; ++x) {
    if (y >= 0 && y < static_cast<int>(height) && 
        x >= 0 && x < static_cast<int>(width)) {
      this->maze[y][x] = ' ';
    }
  }
}

void MazeGenerator::carveVerticalCorridor(int y1, int y2, int x) {
  int minY = std::min(y1, y2);
  int maxY = std::max(y1, y2);
  for (int y = minY; y <= maxY; ++y) {
    if (y >= 0 && y < static_cast<int>(height) && 
        x >= 0 && x < static_cast<int>(width)) {
      this->maze[y][x] = ' ';
    }
  }
}

void MazeGenerator::connectRooms(BSPNode* node, std::default_random_engine& rng) {
  if (!node) return;
  if (!node->left || !node->right) return;
  
  // Recursively connect children first
  connectRooms(node->left, rng);
  connectRooms(node->right, rng);
  
  // Determine connection direction based on how nodes are arranged
  auto leftCenter = getRoomCenter(node->left);
  auto rightCenter = getRoomCenter(node->right);
  
  if (leftCenter.first < 0 || rightCenter.first < 0) return;
  
  // Get room edges facing each other for connection
  int dx = (rightCenter.first > leftCenter.first) ? 1 : -1;
  int dy = (rightCenter.second > leftCenter.second) ? 1 : -1;
  
  // Determine if rooms are more horizontal or vertical from each other
  int horizDist = std::abs(rightCenter.first - leftCenter.first);
  int vertDist = std::abs(rightCenter.second - leftCenter.second);
  
  // Create L-shaped corridor connecting room interiors
  std::uniform_int_distribution<int> coinFlip(0, 1);
  if (coinFlip(rng) == 0) {
    // Horizontal first, then vertical
    carveHorizontalCorridor(leftCenter.first, rightCenter.first, leftCenter.second);
    carveVerticalCorridor(leftCenter.second, rightCenter.second, rightCenter.first);
  } else {
    // Vertical first, then horizontal
    carveVerticalCorridor(leftCenter.second, rightCenter.second, leftCenter.first);
    carveHorizontalCorridor(leftCenter.first, rightCenter.first, rightCenter.second);
  }
}
