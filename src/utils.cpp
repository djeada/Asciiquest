#include "utils.h"
#include "game_settings.h"
#include <algorithm>
#include <random>

Point::Point(int _x, int _y) : x(_x), y(_y) {}
Point::Point() : x(0), y(0) {}
Point::Point(const Point &other) : x(other.x), y(other.y) {}

auto Point::operator==(const Point &p) const -> bool {
  return (x == p.x && y == p.y);
}

/*
int randomFromRange(int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(start, end);
  return dist(gen);
}

int headOrTail() {
  if (randomFromRange(-10001, 10000) >= 1) {
    return 1;
  }
  return -1;
}

void giveValidPositions(int (&myArray)[2], Map myMap) {
  int x = randomFromRange(0, myMap.screenWidth() - 1);
  int y = randomFromRange(0, myMap.screenHeight() - 1);

  while (myMap.getChar(x, y) == BORDER) {
    x = randomFromRange(0, myMap.screenWidth() - 1);
    y = randomFromRange(0, myMap.screenHeight() - 1);
  }

  myArray[0] = x;
  myArray[1] = y;
}

std::vector<Position> findNeighbours(Position cor, int width, int height) {
  std::vector<Position> pos;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (!(i == 0 && j == 0)) {
        if (!(cor.y + i < 0 || cor.y + i >= height)) {
          if (!(cor.x + j < 0 || cor.x + j >= width)) {
            Position neighbourCor = {cor.x + j, cor.y + i};
            pos.push_back(neighbourCor);
          }
        }
      }
    }
  }
  return pos;
}

float distance(const Node &a, const Node &b) {
  return sqrtf((a.cors.x - b.cors.x) * (a.cors.x - b.cors.x) +
               (a.cors.y - b.cors.y) * (a.cors.y - b.cors.y));
}

int partition(std::vector<Node *> &A, int p, int q) {
  Node *pivot = A[q];
  int i = p - 1;
  for (int j = p; j <= q - 1; j++) {
    if (A[j]->distanceTillEnd < pivot->distanceTillEnd) {
      std::swap(A[++i], A[j]);
    }
  }
  // maybe with &
  std::swap(A[i + 1], A[q]);
  return i + 1;
}

void sortByGlobalDistance(std::vector<Node *> &A, int p, int q) {
  if (p < q) {
    int pivot = partition(A, p, q);

    sortByGlobalDistance(A, p, pivot - 1);
    sortByGlobalDistance(A, pivot + 1, q);
  }
}

bool isNodeInVector(std::vector<Node *> &A, Node *key) {
  return std::find(A.begin(), A.end(), key) != A.end();
}

void solveAStar(Node *nodeStart, Node *nodeEnd) {
  Node *nodeCurrent = nodeStart;
  nodeStart->distanceFromStart = 0;
  nodeStart->distanceTillEnd = distance(*nodeStart, *nodeEnd);
  std::vector<Node *> visited;
  std::vector<Node *> notTested;
  notTested.push_back(nodeStart);

  while (!notTested.empty() && nodeCurrent != nodeEnd) {

    sortByGlobalDistance(notTested, 0, notTested.size() - 1);

    while (!notTested.empty() && isNodeInVector(visited, notTested[0]))
      notTested.erase(notTested.begin());

    if (notTested.empty())
      break;

    nodeCurrent = notTested[0];
    visited.push_back(nodeCurrent);

    for (auto neighbour : nodeCurrent->neighbours) {
      if (!isNodeInVector(visited, neighbour) && !neighbour->isBorder)
        notTested.push_back(neighbour);

      float newDistance =
          nodeCurrent->distanceFromStart + distance(*nodeCurrent, *neighbour);

      if (newDistance < neighbour->distanceFromStart) {
        neighbour->parent = nodeCurrent;
        neighbour->distanceFromStart = newDistance;

        neighbour->distanceTillEnd =
            neighbour->distanceFromStart + distance(*neighbour, *nodeEnd);
      }
    }
  }
}

std::vector<Node> initializeNode(Map &myMap) {
  std::vector<Node> nodes;

  int numberVertical = myMap.screenHeight();
  int numberHorizontal = myMap.screenWidth();

  // create Node
  for (int i = 0; i < numberVertical; i++) {
    for (int j = 0; j < numberHorizontal; j++) {
      if (myMap.getChar(j, i) == BORDER) {
        Node new_node = {true, INFINITY, INFINITY, {j, i}, {}, nullptr};
        nodes.push_back(new_node);
      } else {
        Node new_node = {false, INFINITY, INFINITY, {j, i}, {}, nullptr};
        nodes.push_back(new_node);
      }
    }
  }

  return nodes;
}

std::vector<Position> getRoute(Node *nodeStart, Node *nodeEnd) {
  std::vector<Position> route;
  Node *nodeCurrent = nodeEnd;
  while (nodeCurrent != nodeStart) {
    route.push_back(nodeCurrent->cors);
    nodeCurrent = nodeCurrent->parent;
  }

  return route;
}

std::vector<Position> findTheRoute(int playerX, int playerY, int monsterX,
                                   int monsterY, Map &myMap) {
  std::vector<Node> nodes = initializeNode(myMap);

  // initalize neighbours
  int numberVertical = myMap.screenHeight();
  int numberHorizontal = myMap.screenWidth();
  for (int i = 0; i < numberVertical; i++) {
    for (int j = 0; j < numberHorizontal; j++) {
      std::vector<Position> neighboursCors =
          findNeighbours(nodes[j + i * numberHorizontal].cors, numberHorizontal,
                         numberVertical);
      for (auto &cor : neighboursCors)
        nodes[j + i * numberHorizontal].neighbours.push_back(
            &nodes[cor.x + cor.y * numberHorizontal]);
    }
  }

  Node *nodeStart = &nodes[monsterX + monsterY * numberHorizontal];
  Node *nodeEnd = &nodes[playerX + playerY * numberHorizontal];

  solveAStar(nodeStart, nodeEnd);

  return getRoute(nodeStart, nodeEnd);
}
*/
