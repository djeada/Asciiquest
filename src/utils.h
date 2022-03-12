#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_UTILS_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_UTILS_H

#include <cstddef>
#include <functional>
#include <string>

struct Point {
  int x;
  int y;
  Point(int _x, int _y);
  Point();
  Point(const Point &other);
  auto operator==(const Point &p) const -> bool;
  auto operator!=(const Point &p) const -> bool;
  auto distance(const Point &p) const -> double;
  auto toString() const -> std::string;
};

// Mapping two integers to one, in a unique and deterministic way
auto hash_value(const Point &p) -> size_t;

// hash function for Point
namespace std {
template <> struct hash<Point> {
  auto operator()(const Point &p) const -> size_t;
};
} // namespace std

#endif
