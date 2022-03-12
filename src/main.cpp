#include "game.h"
#include <csignal>
#include <cstdlib>
#include <ncurses.h>

void screenSetUp() {
  initscr();
  noecho();
  refresh();
  start_color();
}

void exitCleanup(int /*_*/) {
  endwin();
  std::system("clear");
  exit(0);
}

auto main() -> int {
  signal(SIGQUIT, exitCleanup);

  screenSetUp();
  Game game;
  game.run();

  return 0;
}
