#include "game_over_renderer.h"
#include <algorithm>
#include <ncurses.h>
#include <string>

GameOverRenderer::GameOverRenderer(const RendererData &_data) : data(_data) {}
GameOverRenderer::~GameOverRenderer() {}

void GameOverRenderer::draw() {
  // First, draw the game board content
  std::unique_ptr<GameBoardRenderer> gameBoardRenderer =
      std::make_unique<GameBoardRenderer>(data);
  gameBoardRenderer->drawContent();
  
  // Then draw "Game Over" on top
  drawGameOver();
  
  // Finally, apply all changes at once for double buffering
  wnoutrefresh(stdscr);
  doupdate();
}

void GameOverRenderer::drawGameOver() {
  getmaxyx(stdscr, termHeight, termWidth);
  std::string gameOver = "GAME OVER";
  std::string score = "Score: " + data.stats["Score"];
  std::string dungeon = "Dungeon Level: " + data.stats["DungeonLevel"];
  std::string hint = "Press Enter/Space for Menu, Q/ESC to Quit";

  int boxWidth = std::min(50, termWidth - 4);
  boxWidth = std::max(10, boxWidth);
  boxWidth = std::min(boxWidth, termWidth - 2);
  int boxHeight = 7;
  int boxTop = std::max(1, (termHeight - boxHeight) / 2);
  int boxLeft = std::max(2, (termWidth - boxWidth) / 2);

  // Clear the interior of the box first
  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_BORDER)));
  for (int y = boxTop + 1; y < boxTop + boxHeight - 1; ++y) {
    mvhline(y, boxLeft + 1, ' ', boxWidth - 2);
  }
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_BORDER)));

  // Draw the box border
  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_BORDER)));
  mvaddch(boxTop, boxLeft, ACS_ULCORNER);
  mvaddch(boxTop, boxLeft + boxWidth - 1, ACS_URCORNER);
  mvaddch(boxTop + boxHeight - 1, boxLeft, ACS_LLCORNER);
  mvaddch(boxTop + boxHeight - 1, boxLeft + boxWidth - 1, ACS_LRCORNER);
  mvhline(boxTop, boxLeft + 1, ACS_HLINE, boxWidth - 2);
  mvhline(boxTop + boxHeight - 1, boxLeft + 1, ACS_HLINE, boxWidth - 2);
  mvvline(boxTop + 1, boxLeft, ACS_VLINE, boxHeight - 2);
  mvvline(boxTop + 1, boxLeft + boxWidth - 1, ACS_VLINE, boxHeight - 2);
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_BORDER)));

  int textX = boxLeft + 2;
  int textY = boxTop + 1;

  attron(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::PLAYER)));
  mvprintw(textY++, textX, "%s", gameOver.c_str());
  attroff(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::PLAYER)));

  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
  mvprintw(textY++, textX, "%s", score.c_str());
  mvprintw(textY++, textX, "%s", dungeon.c_str());
  mvprintw(textY + 1, textX, "%s", hint.c_str());
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
}
