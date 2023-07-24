#include "main_menu_renderer.h"
#include <ncurses.h>

MainMenuRenderer::MainMenuRenderer() {}

MainMenuRenderer::~MainMenuRenderer() {}

void MainMenuRenderer::draw(

) {

  clear(); // Clear the screen

  // Draw the title
  mvprintw(0, 0, "Main Menu");

  // Draw the menu options
  mvprintw(2, 0, "1. Start game");
  mvprintw(3, 0, "2. Settings");
  mvprintw(4, 0, "3. Exit");

  refresh(); // Refresh the screen to apply the changes
}
