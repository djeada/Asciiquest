#include "utils.h"
#include "game_settings.h"
#include <algorithm>
#include <cmath>

Point::Point(int _x, int _y) : x(_x), y(_y) {}
Point::Point() : x(0), y(0) {}
Point::Point(const Point &other) : x(other.x), y(other.y) {}

auto Point::operator==(const Point &p) const -> bool {
  return (x == p.x && y == p.y);
}

auto Point::operator!=(const Point &p) const -> bool { return !(*this == p); }

auto Point::distance(const Point &p) const -> double {
  // use the Pythagorean theorem
  return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2));
}

auto Point::toString() const -> std::string {
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

auto hash_value(const Point &p) -> size_t {
  // use the Cantor pairing function
  // https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
  return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.y;
}
namespace std {
auto hash<Point>::operator()(const Point &p) const -> size_t {
  return hash_value(p);
}
} // namespace std