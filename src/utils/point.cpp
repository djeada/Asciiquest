#include "point.h"
#include "utils/game_settings.h"
#include <algorithm>
#include <cmath>

// Constructor
Point::Point(int _x, int _y) : x(_x), y(_y) {}

// Default constructor
Point::Point() : x(0), y(0) {}

// Copy constructor
Point::Point(const Point &other) : x(other.x), y(other.y) {}

// Equality comparison operator
bool Point::operator==(const Point &p) const { return (x == p.x && y == p.y); }

// Inequality comparison operator
bool Point::operator!=(const Point &p) const { return !(*this == p); }

// Addition assignment operator
Point &Point::operator+=(const Point &other) {
  this->x += other.x;
  this->y += other.y;
  return *this;
}

// Function to calculate the distance between two points
double Point::distance(const Point &p) const {
  return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2));
}

// Function to convert the point to a string
std::string Point::toString() const {
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

// Function to get the hash value of the point
size_t hash_value(const Point &p) {
  return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.y;
}

// Hash function for Point
namespace std {
size_t hash<Point>::operator()(const Point &p) const { return hash_value(p); }
} // namespace std
