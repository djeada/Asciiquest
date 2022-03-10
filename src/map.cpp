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
      j = ' ';
    }
  }
}

auto intToStr(int value) -> std::string {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

void displayPlayerInfo(int y, int health, int maxHealth, int level, int exp, int nextExp,
                       int currentLevel) {
  int x = 0;

  std::string message = "Health: " + intToStr(health) + "/" + intToStr(maxHealth) +
                " Exp: " + intToStr(exp) + "/" + intToStr(nextExp) + " Level: " + intToStr(level);

  char const *pchar = message.c_str();

  mvprintw(y, x, pchar);
}

void Map::draw(const Player &player) {
  int y = 0;
  int x = 0;
  for (auto &row : map) {
    const char *s = row.c_str();
    mvprintw(y, x, s);
    y++;
  }
  displayPlayerInfo(y, player.getHealth(), player.getMaxHealth(),
                    player.getLevel(), player.getExp(), player.expToNextLevel(), 1);
}

auto Map::getChar(const Point &point) -> char { return map[point.y][point.x]; }

auto Map::screenWidth() const -> int { return width; }

auto Map::screenHeight() const -> int { return height; }

auto Map::isPositionFree(const Point &point) -> bool {
  if (point.x < 0 || point.x >= screenWidth())
    return false;

  if (point.y < 0 || point.y >= screenHeight())
    return false;

  return getChar(point) == ' ';
}
auto Map::randomFreePosition() -> Point {
  int x = 0;
  int y = 0;
  do {
    x = rand() % screenWidth();
    y = rand() % screenHeight();
  } while (!isPositionFree(Point(x, y)));
  return Point(x, y);
}

auto Map::getStart() -> Point { return start; }

auto Map::getEnd() -> Point { return end; }
