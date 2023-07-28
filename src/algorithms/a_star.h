#ifndef A_STAR_H
#define A_STAR_H

#include <cmath>
#include <deque>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include "utils/point.h"

template <typename T> class AStar {
public:
  struct Node {
    Point point;
    Point cameFrom;
    int costFromStart = std::numeric_limits<int>::max();
    int totalEstimatedCost = std::numeric_limits<int>::max();

    Node() = default;
    Node(Point p, Point cf, int g, int f)
        : point(std::move(p)), cameFrom(std::move(cf)), costFromStart(g), totalEstimatedCost(f) {}
  };

private:
  std::deque<Point> bestPath;
  std::function<bool(T)> isNavigable;

  std::vector<Point> getNeighbors(const Point &p, const std::vector<std::vector<T>> &grid) {
    std::vector<Point> neighbors;
    const auto px = p.x;
    const auto py = p.y;

    if (px > 0 && isNavigable(grid[py][px - 1]))
      neighbors.emplace_back(px - 1, py);

    if (px < grid[0].size() - 1 && isNavigable(grid[py][px + 1]))
      neighbors.emplace_back(px + 1, py);

    if (py > 0 && isNavigable(grid[py - 1][px]))
      neighbors.emplace_back(px, py - 1);

    if (py < grid.size() - 1 && isNavigable(grid[py + 1][px]))
      neighbors.emplace_back(px, py + 1);

    return neighbors;
  }

  static int heuristic(const Point &a, const Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }

public:
  AStar(
      const std::vector<std::vector<T>> &grid, Point start,
      Point end,
      std::function<bool(T)> isNavigableFunc = [](T value) { return static_cast<bool>(value); })
      : isNavigable(std::move(isNavigableFunc)) {
    solve(grid, std::move(start), std::move(end));
  }

  void solve(const std::vector<std::vector<T>> &grid, Point start, Point end) {
    std::unordered_map<Point, Node> nodes;
    std::unordered_set<Point> visited;
    std::priority_queue<Point, std::vector<Point>, std::function<bool(Point, Point)>> queue(
        [&nodes](Point a, Point b) {
          return nodes[a].totalEstimatedCost > nodes[b].totalEstimatedCost;
        });

    nodes[start] = Node(start, {}, 0, heuristic(start, end));
    queue.push(start);

    while (!queue.empty()) {
      Point current = queue.top();
      queue.pop();

      // If the node was already visited, we ignore it
      if (visited.count(current))
        continue;

      visited.insert(current);

      if (current == end) {
        while (current != start) {
          bestPath.push_front(current);
          current = nodes[current].cameFrom;
        }

        bestPath.push_front(start);
        return;
      }

      for (const auto &neighbor : getNeighbors(current, grid)) {
        if (visited.count(neighbor))
          continue; // Ignore if neighbor was already visited

        int tentativeCostFromStart = nodes[current].costFromStart + 1; // Assuming all moves cost 1

        if (!nodes.count(neighbor) || tentativeCostFromStart < nodes[neighbor].costFromStart) {
          nodes[neighbor] = Node(neighbor, current, tentativeCostFromStart, tentativeCostFromStart + heuristic(neighbor, end));
          queue.push(neighbor);
        }
      }
    }
  }

  [[nodiscard]] auto getPath() const -> std::deque<Point> { return bestPath; }
};

#endif // A_STAR_H

