#ifndef _UTILS_POINT_H
#define _UTILS_POINT_H

#include <cstddef>
#include <functional>
#include <string>

struct Point {
  /**
   * @brief Represents a point in a 2D plane.
   */
  int x;
  int y;

  Point(int _x, int _y);     // constructs a point with the given coordinates
  Point();                   // constructs a point with 0, 0 coordinates
  Point(const Point &other); // copy constructor

  bool operator==(const Point &p) const; // compares two points for equality
  bool operator!=(const Point &p) const; // compares two points for inequality
  Point &operator+=(const Point &p); // performs addition assignment on a point

  double
  distance(const Point &p) const; // calculates the distance between two points
  std::string toString() const;   // converts the point to a string
};

// mapping two integers to one, in a unique and deterministic way
size_t hash_value(const Point &p);

// hash function for Point
namespace std {
template <> struct hash<Point> { size_t operator()(const Point &p) const; };
} // namespace std

#endif
