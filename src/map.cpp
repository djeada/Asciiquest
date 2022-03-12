#include "map.h"
#include "game_settings.h"
#include <fstream>
#include <ncurses.h>
#include <sstream>

Map::Map(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) {}

void Map::loadLevel() {
  auto generator =
      MazeGenerator(width, height, MazeGeneratorAlgorithm::DepthFirstSearch);
  map = generator.getMaze();
  auto startingPoint = generator.getStart();
  start = Point(startingPoint.first, startingPoint.second);
  auto endPoint = generator.getEnd();
  end = Point(endPoint.first, endPoint.second);
}

void Map::clear() {
  for (auto &i : map) {
    for (char &j : i) {
      j = Symbols::emptySpace;
    }
  }
}

void Map::draw(const Player &player) {

  auto displayVector = [](int y, int x, std::vector<std::string> &info) {
    for (auto &i : info) {
      const char *pchar = i.c_str();
      mvprintw(y, x, pchar);
      y++;
    }
  };

  auto displayPlayerInfo = [](int y, int x, const PlayerInfo &playerInfo) {
    std::string message = "Health: " + playerInfo.health + "/" +
                          playerInfo.maxHealth + " Level: " + playerInfo.level +
                          " Exp: " + playerInfo.exp + "/" + playerInfo.nextExp;

    char const *pchar = message.c_str();
    mvprintw(y, x, pchar);
  };

  displayVector(0, 0, map);
  displayPlayerInfo(screenHeight(), 0, PlayerInfo(player));
  displayVector(0, screenWidth() + 1, fightInfo);
}

auto Map::getChar(const Point &point) const -> char {
  return map[point.y][point.x];
}

auto Map::screenWidth() const -> int { return width; }

auto Map::screenHeight() const -> int { return height; }

auto Map::isPositionFree(const Point &point) const -> bool {
  if (point.x < 0 || point.x >= screenWidth())
    return false;

  if (point.y < 0 || point.y >= screenHeight())
    return false;

  return getChar(point) == Symbols::emptySpace;
}

auto Map::randomFreePosition() const -> Point {
  int x = 0;
  int y = 0;
  do {
    x = rand() % screenWidth();
    y = rand() % screenHeight();
  } while (!isPositionFree(Point(x, y)));
  return Point(x, y);
}

auto Map::getStart() const -> Point { return start; }

auto Map::getEnd() const -> Point { return end; }

void Map::setFightInfo(const std::vector<std::string> &info) {
  fightInfo = info;
}

auto Map::getNeighbours(const Point &point) const -> std::vector<Point> {
  std::vector<Point> neighbours;
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      if (x == 0 && y == 0)
        continue;
      Point neighbour(point.x + x, point.y + y);
      if (isPositionFree(neighbour))
        neighbours.push_back(neighbour);
    }
  }
  return neighbours;
}

auto Map::distance(const Point &point1, const Point &point2) const -> double {
  // if point2 is a non-diagonal neighbour of point1, return 1
  // if point2 is a diagonal neighbour of point1, return sqrt(2)
  // if point2 is not a neighbour return std::numeric_limits<double>::max()
  if (point1.x == point2.x) {
    if (point1.y == point2.y)
      return 1;
    else if (point1.y == point2.y + 1 || point1.y == point2.y - 1)
      return 1.41421356237;
    else
      return std::numeric_limits<double>::max();
  } else if (point1.y == point2.y) {
    if (point1.x == point2.x + 1 || point1.x == point2.x - 1)
      return 1.41421356237;
    else
      return std::numeric_limits<double>::max();
  } else if (point1.x == point2.x + 1 && point1.y == point2.y + 1)
    return 1.41421356237;
  else if (point1.x == point2.x - 1 && point1.y == point2.y - 1)
    return 1.41421356237;
  else if (point1.x == point2.x + 1 && point1.y == point2.y - 1)
    return 1.41421356237;
  else if (point1.x == point2.x - 1 && point1.y == point2.y + 1)
    return 1.41421356237;
  else
    return std::numeric_limits<double>::max();
}