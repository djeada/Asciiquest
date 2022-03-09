#include "game.h"
#include <csignal>
#include <cstdlib>
#include <ncurses.h>

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

void exitCleanup(int _) {
  endwin();
  std::system("clear");
  exit(0);
}

int main() {
  signal(SIGQUIT, exitCleanup);

  screenSetUp();
  Game game;
  game.run();

  return 0;
}
