#include "main_menu_renderer.h"
#include "game_board_renderer.h"
#include <algorithm>
#include <ncurses.h>
#include <string>
#include <vector>

namespace {
void drawCenteredText(int row, const std::string &text, int width, int attrs) {
  if (width <= 0) {
    return;
  }
  int start = std::max(0, (width - static_cast<int>(text.size())) / 2);
  attron(attrs);
  mvprintw(row, start, "%s", text.c_str());
  attroff(attrs);
}

void drawFrame(int top, int left, int height, int width, int colorPair) {
  if (height < 3 || width < 3) {
    return;
  }
  attron(COLOR_PAIR(colorPair));
  mvaddch(top, left, ACS_ULCORNER);
  mvaddch(top, left + width - 1, ACS_URCORNER);
  mvaddch(top + height - 1, left, ACS_LLCORNER);
  mvaddch(top + height - 1, left + width - 1, ACS_LRCORNER);
  mvhline(top, left + 1, ACS_HLINE, width - 2);
  mvhline(top + height - 1, left + 1, ACS_HLINE, width - 2);
  mvvline(top + 1, left, ACS_VLINE, height - 2);
  mvvline(top + 1, left + width - 1, ACS_VLINE, height - 2);
  attroff(COLOR_PAIR(colorPair));
}
} // namespace

MainMenuRenderer::MainMenuRenderer() {}

MainMenuRenderer::~MainMenuRenderer() {}

void MainMenuRenderer::draw() {
  int termHeight;
  int termWidth;
  getmaxyx(stdscr, termHeight, termWidth);
  start_color();
  init_pair(static_cast<int>(ColorPair::UI_BORDER), COLOR_WHITE, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::UI_TITLE), COLOR_YELLOW, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::UI_TEXT), COLOR_WHITE, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::UI_ACCENT), COLOR_CYAN, COLOR_BLACK);

  std::vector<std::string> title = {"ASCIIQUEST", "A Roguelike Adventure"};
  int titleStart = std::max(1, termHeight / 6);
  drawCenteredText(titleStart, title[0], termWidth,
                   A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::UI_TITLE)));
  drawCenteredText(titleStart + 1, title[1], termWidth,
                   COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));

  int boxWidth = std::min(50, termWidth - 4);
  boxWidth = std::max(20, boxWidth);
  boxWidth = std::min(boxWidth, termWidth - 2);
  int boxHeight = 9;
  int boxTop = std::max(2, std::min(termHeight - boxHeight - 2, titleStart + 3));
  int boxLeft = std::max(2, (termWidth - boxWidth) / 2);

  drawFrame(boxTop, boxLeft, boxHeight, boxWidth,
            static_cast<int>(ColorPair::UI_BORDER));

  int line = boxTop + 2;
  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
  mvprintw(line++, boxLeft + 3, "1. Start Game");
  mvprintw(line++, boxLeft + 3, "2. Options");
  mvprintw(line++, boxLeft + 3, "3. Quit");
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));

  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));
  mvprintw(line + 1, boxLeft + 3, "Tip: Use WASD/Arrows to move.");
  mvprintw(line + 2, boxLeft + 3, "Press 1-5 to cast spells.");
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));

  drawCenteredText(termHeight - 2, "Press a number to continue", termWidth,
                   COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));

  wnoutrefresh(stdscr); // Update virtual screen
  doupdate(); // Apply all changes at once for double buffering
}
