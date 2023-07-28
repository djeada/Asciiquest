#ifndef _UTILS_POINT_H
#define _UTILS_POINT_H

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

struct Point {
  /**
   * @brief Represents a point in a 2D plane.
   */
  int x;
  int y;

  Point(int _x = 0, int _y = 0); // constructs a point with the given
                                 // coordinates or 0,0 by default
  Point(const Point &other);     // copy constructor

  bool operator==(const Point &p) const; // compares two points for equality
  bool operator!=(const Point &p) const; // compares two points for inequality

  Point &operator+=(const Point &p); // performs addition assignment on a point
  Point operator+(const Point &p) const;

  Point &
  operator-=(const Point &p); // performs subtraction assignment on a point
  Point operator-(const Point &p) const;

  Point &
  operator*=(int scalar); // performs multiplication assignment on a point
  Point operator*(int scalar) const;

  Point &operator/=(int scalar); // performs division assignment on a point
  Point operator/(int scalar) const;

  double
  distance(const Point &p) const; // calculates the distance between two points
  std::string toString() const;   // converts the point to a string
};

std::ostream &operator<<(std::ostream &os, const Point &p);

namespace std {
template <> struct hash<Point> {
  size_t operator()(const Point &p) const noexcept;
};
} // namespace std

#endif
