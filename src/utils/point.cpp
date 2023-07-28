#include "point.h"
#include <algorithm>
#include <cmath>

Point::Point(int _x, int _y) : x(_x), y(_y) {}

Point::Point(const Point &other) : x(other.x), y(other.y) {}

bool Point::operator==(const Point &p) const { return x == p.x && y == p.y; }

bool Point::operator!=(const Point &p) const { return !(*this == p); }

Point &Point::operator+=(const Point &p) {
  x += p.x;
  y += p.y;
  return *this;
}

Point Point::operator+(const Point &p) const {
  Point result = *this;
  result += p;
  return result;
}

Point &Point::operator-=(const Point &p) {
  x -= p.x;
  y -= p.y;
  return *this;
}

Point Point::operator-(const Point &p) const {
  Point result = *this;
  result -= p;
  return result;
}

Point &Point::operator*=(int scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Point Point::operator*(int scalar) const {
  Point result = *this;
  result *= scalar;
  return result;
}

Point &Point::operator/=(int scalar) {
  if (scalar != 0) {
    x /= scalar;
    y /= scalar;
  }
  return *this;
}

Point Point::operator/(int scalar) const {
  Point result = *this;
  if (scalar != 0) {
    result /= scalar;
  }
  return result;
}

double Point::distance(const Point &p) const {
  int dx = x - p.x;
  int dy = y - p.y;
  return std::sqrt(dx * dx + dy * dy);
}

std::string Point::toString() const {
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

std::ostream &operator<<(std::ostream &os, const Point &p) {
  os << "Point(" << p.x << ", " << p.y << ")";
  return os;
}

namespace std {
size_t hash<Point>::operator()(const Point &p) const noexcept {
  // This hash function is just a basic example and may not provide the best
  // distribution
  return hash<int>()(p.x) ^ hash<int>()(p.y);
}
} // namespace std
