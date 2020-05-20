#include "map.h"
#include "game_settings.h"
#include <fstream>
#include <ncurses.h>
#include <sstream>

const char *maps[number_of_levels] = {"maps/map_1.txt", "maps/map_2.txt",
                                      "maps/map_3.txt"};

Map::Map() {
  currentLevel = 0;
  loadTheMap(maps[currentLevel]);
}

void Map::levelUp() {
  currentLevel++;
  if (currentLevel != number_of_levels) {
    rows.clear();
    loadTheMap(maps[currentLevel]);
  }
}

void Map::loadTheMap(std::string path) {
  std::ifstream in;
  in.open(path);
  std::string row;
  while (in) {
    std::getline(in, row);
    rows.push_back(row);
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

void Map::draw(int health, int level, int exp) {
  int y = 0;
  int x = 0;
  for (auto &row : rows) {
    const char *s = row.c_str();
    mvprintw(y, x, s);
    y += 1;
  }
  displayPlayerInfo(y, health, level, exp, currentLevel);
}

char Map::getChar(int x, int y) { return rows[y][x]; }

int Map::getLevel() { return currentLevel; }

int Map::screenWidth() { return rows[0].size(); }

int Map::screenHeight() { return rows.size(); }
