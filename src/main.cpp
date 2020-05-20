#include "monster.h"
#include "player.h"
#include "treasure.h"
#include <ncurses.h>

void screenSetUp();
bool gameLogic(Player *p, std::vector<Monster> &monsters,
               std::vector<Treasure> &treasures);

int main() {

  Player myPlayer(1, 1);

  std::vector<Monster> myMonsters = initalizeMonsters(
      (myPlayer.getMap().getLevel() + 1) * 10, myPlayer.getMap());

  std::vector<Treasure> myTreasures = initalizeTreasures(3, myPlayer.getMap());

  screenSetUp();

  while (gameLogic(&myPlayer, myMonsters, myTreasures)) {
  }

  endwin();

  return 0;
}

void screenSetUp() {
  initscr();
  noecho();
  refresh();
  start_color();
  init_pair(PLAYER_COLOR, COLOR_GREEN, COLOR_BLACK);
  init_pair(TREASURE_COLOR, COLOR_CYAN, COLOR_BLACK);
  init_pair(GOBLIN_COLOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(ORC_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(TROLL_COLOR, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(DRAGON_COLOR, COLOR_YELLOW, COLOR_BLACK);
}

bool gameLogic(Player *p, std::vector<Monster> &monsters,
               std::vector<Treasure> &treasures) {

  clear();
  p->getMap().draw(p->getHealth(), p->getLevel(), p->getExp());

  for (auto &m : monsters) {
    m.draw();
  }

  for (auto &t : treasures) {
    t.draw();
  }

  p->draw();

  char ch = getch();

  if (ch == esc)
    return false;

  if (p->getHealth() > 0 && p->getMap().getLevel() != number_of_levels) {
    p->handleInput(ch);
    p->fight(monsters);
    p->checkTreasure(treasures);

    for (auto &m : monsters) {
      m.moveMonster(p->getMap(), p->getX(), p->getY());
    }
  }

  if (p->newMapLevel()) {
    monsters =
        initalizeMonsters((p->getMap().getLevel() + 1) * 10, p->getMap());
    treasures =
        initalizeTreasures((p->getMap().getLevel() + 1) * 3, p->getMap());

    p->turnOffMapLevelFlag();
  }

  return true;
}
