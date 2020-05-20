#include "treasure.h"
#include "game_settings.h"
#include "utils.h"
#include <ncurses.h>

Treasure::Treasure(int _x, int _y) {
  x = _x;
  y = _y;
  bonus_attack = randomFromRange(0, 2);
  bonus_exp = randomFromRange(0, 10);
  bonus_health = randomFromRange(0, 100);
}

void Treasure::draw() {
  attron(COLOR_PAIR(TREASURE_COLOR));
  mvprintw(y, x, TREASURE_TILE);
  attroff(COLOR_PAIR(TREASURE_COLOR));
}

int Treasure::getX() { return x; }

int Treasure::getY() { return y; }

int Treasure::getBonusAttack() { return bonus_attack; }

int Treasure::getBonusExp() { return bonus_exp; }

int Treasure::getBonusHealth() { return bonus_health; }

std::vector<Treasure> initalizeTreasures(int how_many, Map myMap) {
  std::vector<Treasure> treasures;
  int pos[2] = {};

  for (int i = 0; i < how_many; i++) {
    giveValidPositions(pos, myMap);
    treasures.push_back(Treasure(pos[0], pos[1]));
  }

  return treasures;
}
