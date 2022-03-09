#include "map.h"
#include "game_settings.h"
#include <fstream>
#include <ncurses.h>
#include <sstream>

// commen
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
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      map[i][j] = ' ';
    }
  }
}

std::string intToStr(int value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

void displayPlayerInfo(int y, int health, int level, int exp,
                       int currentLevel) {
  int x = 0;

  std::string message = "";

  if (health > 0) {

    if (currentLevel != number_of_levels) {
      message = "Health: " + intToStr(health) + "/" +
                intToStr((level + 1) * 10) + " Level: " + intToStr(level) +
                " Exp: " + intToStr(exp);
    } else {
      message = "You won!";
    }
  }

  else {
    message = "You died!";
  }

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
  displayPlayerInfo(y, player.getHealth(), player.getLevel(), player.getExp(),
                    1);
}

char Map::getChar(Point point) { return map[point.y][point.x]; }

int Map::screenWidth() { return width; }

int Map::screenHeight() { return height; }

bool Map::isPositionFree(Point point) { return getChar(point) == ' '; }
Point Map::randomFreePosition() {
  int x = 0;
  int y = 0;
  do {
    x = rand() % screenWidth();
    y = rand() % screenHeight();
  } while (!isPositionFree(Point(x, y)));
  return Point(x, y);
}

Point Map::getStart() { return start; }

Point Map::getEnd() { return end; }
