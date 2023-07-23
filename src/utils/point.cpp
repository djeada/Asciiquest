#include "point.h"
#include "utils/game_settings.h"
#include <algorithm>
#include <cmath>

Point::Point(int _x, int _y) : x(_x), y(_y) {
  /**
   * @brief Constructs a point with the given coordinates.
   * @param _x The x coordinate.
   * @param _y The y coordinate.
   * @return A point with the given coordinates.
   */
}
Point::Point() : x(0), y(0) {
  /**
   * @brief Constructs a point with 0, 0 coordinates.
   * @return A point with 0, 0 coordinates.
   */
}
Point::Point(const Point &other) : x(other.x), y(other.y) {
  /**
   * @brief Constructs a point with the same coordinates as the given point.
   * @param other The point to copy.
   * @return A point with the same coordinates as the given point.
   */
}

auto Point::operator==(const Point &p) const -> bool {
  /**
   * @brief Compares two points for equality.
   * @param p The point to compare to.
   * @return True if the points are equal, false otherwise.
   */
  return (x == p.x && y == p.y);
}

auto Point::operator!=(const Point &p) const -> bool {
  /**
   * @brief Compares two points for inequality.
   * @param p The point to compare to.
   * @return True if the points are not equal, false otherwise.
   */
  return !(*this == p);
}

auto Point::distance(const Point &p) const -> double {
  /**
   * @brief Calculates the distance between two points.
   * @param p The point to calculate the distance to.
   * @return The distance between the two points.
   */
  // use the Pythagorean theorem
  return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2));
}

auto Point::toString() const -> std::string {
  /**
   * @brief Converts the point to a string.
   * @return The point as a string.
   */
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

auto hash_value(const Point &p) -> size_t {
  /**
   * @brief Maps a point to a unique and deterministic value.
   * @param p The point to map.
   * @return The unique and deterministic value of the point.
   */
  // use the Cantor pairing function
  // https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
  return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.y;
}
namespace std {
auto hash<Point>::operator()(const Point &p) const -> size_t {
  /**
   * @brief Hash function for Point.
   * @param p The point to hash.
   * @return The hash value of the point.
   */
  return hash_value(p);
}
} // namespace std
