#include "a_star.h"
#include "game_settings.h"
#include <functional>
#include <queue>
#include <random>
#include <stdexcept>
#include <unordered_map>

AStar::AStar(const Map &map, const Point &start, const Point &end) {
  solveAStar(map, start, end);
}

void AStar::solveAStar(const Map &map, const Point &start, const Point &end) {

  // clear the bestPath vector
  bestPath.clear();
  bestPath.push_back(start);

  // create a heuristic function used to assess the distance from a point till
  // the end
  auto heuristic = [&end](const Point &p) { return p.distance(end); };

  // check if the start and end points are in the map
  if (start.x < 0 || start.x >= map.screenWidth() || start.y < 0 ||
      start.y >= map.screenHeight() || end.x < 0 ||
      end.x >= map.screenWidth() || end.y < 0 || end.y >= map.screenHeight()) {
    throw std::runtime_error("start or end point is not in the map");
  }

  if (start == end) {
    return;
  }

  // create 3 unordered maps to store the nodes
  // the first map stores the current distances from the start point
  // the second map stores the current distances till the end point
  // the third map stores the boolean values indicating if the node has been
  // visited
  std::unordered_map<Point, int> distanceFromStart;
  std::unordered_map<Point, int> distanceToEnd;
  std::unordered_map<Point, bool> visited;

  // iterate over all nodes in the map and set the distances to infity and
  // visited to false
  for (int y = 0; y < map.screenHeight(); y++) {
    for (int x = 0; x < map.screenWidth(); x++) {
      distanceFromStart[Point(x, y)] = std::numeric_limits<int>::max();
      distanceToEnd[Point(x, y)] = std::numeric_limits<int>::max();
      visited[Point(x, y)] = false;
    }
  }

  // set the distance from the start point to 0
  distanceFromStart[start] = 0;

  // set the distance from the end point to 0
  distanceToEnd[end] = 0;

  // set the distance from the end point to 0
  distanceToEnd[end] = 0;

  // create a priority queue to store the nodes
  // the priority queue is sorted by the distance from the start point
  std::priority_queue<Point, std::vector<Point>,
                      std::function<bool(Point, Point)>>
      queue(std::function<bool(const Point &, const Point &)>(
          [&distanceToEnd](const Point &a, const Point &b) {
            return distanceToEnd[a] > distanceToEnd[b];
          }));

  // add the start point to the queue
  queue.push(start);

  // while the queue is not empty
  while (!queue.empty()) {
    // get the node with the smallest distance from the start point
    auto current = queue.top();
    queue.pop();
    // check if current is the end point
    if (current == end) {
      // if it is, return the best path
      // the best path is the path with the smallest distance from the start
      // point now we have to reconstruct it using distanceFromStart
      while (current != start) {
        // check which of the neighbours has the smallest distance from the
        // start
        auto neighbours = map.getNeighbours(current);
        // shuffle the neighbours vector
        std::random_shuffle(neighbours.begin(), neighbours.end());
        int minDistance = std::numeric_limits<int>::max();
        Point minNeighbour;
        for (auto neighbour : neighbours) {
          if (distanceFromStart[neighbour] < minDistance) {
            minDistance = distanceFromStart[neighbour];
            minNeighbour = neighbour;
          }
        }
        // add the neighbour to the best path
        bestPath.push_front(minNeighbour);
        // set the current node to the neighbour
        current = minNeighbour;
      }
      return;
    }

    // get the neighbours of the current node
    auto neighbours = map.getNeighbours(current);

    // iterate over the neighbours
    for (auto neighbour : neighbours) {

      auto tentativeDistanceFromStart =
          distanceFromStart[current] + map.distance(current, neighbour);
      if (tentativeDistanceFromStart < distanceFromStart[neighbour]) {
        // set the distance from the start point to the neighbour
        distanceFromStart[neighbour] = tentativeDistanceFromStart;
        // set the distance from the end point to the neighbour
        distanceToEnd[neighbour] =
            distanceFromStart[neighbour] + heuristic(neighbour);

        if (!visited[neighbour]) {
          // add the neighbour to the queue
          queue.push(neighbour);
          // set the neighbour as visited
          visited[neighbour] = true;
        }
      }
    }
  }
}

auto AStar::getPath() const -> std::deque<Point> { return bestPath; }
