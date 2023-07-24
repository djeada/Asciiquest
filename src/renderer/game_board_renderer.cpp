#include "game_board_renderer.h"
#include <ncurses.h>

std::unordered_map<CellType, std::pair<char, int>> cellTypeToCharColor = {
    {CellType::EMPTY, {' ', 1}},
    {CellType::WALL, {'#', 2}},
    {CellType::PLAYER, {'@', 3}}};

GameBoardRenderer::GameBoardRenderer(const RendererData &_data) : data(_data) {
  start_color(); // Start color functionality

  // Define color pairs
  if (!has_colors()) {
    return;
  }

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  // Get terminal size
  getmaxyx(stdscr, termHeight, termWidth);

  // Calculate sizes
  boardWidth = termWidth * 0.75;
  fightInfoWidth = termWidth * 0.25;
}

GameBoardRenderer::~GameBoardRenderer() {}

void GameBoardRenderer::draw() {
  clear();
  drawBoard();
  drawFightInfo();
  drawStats();
  refresh();
}

void GameBoardRenderer::drawBoard() {
  const auto &grid = data.grid;
  const auto &playerPosition = data.playerPosition; // assuming this exists
  int boardHeight =
      std::min(termHeight - 5,
               static_cast<int>(grid.size())); // reserve 5 lines for stats
  int boardWidth = std::min(this->boardWidth, static_cast<int>(grid[0].size()));

  // calculate the top-left corner of the view
  int viewTop =
      std::max(0, std::min(static_cast<int>(grid.size()) - boardHeight,
                           playerPosition.y - boardHeight / 2));
  int viewLeft =
      std::max(0, std::min(static_cast<int>(grid[0].size()) - boardWidth,
                           playerPosition.x - boardWidth / 2));

  for (int y = 0; y < boardHeight; y++) {
    for (int x = 0; x < boardWidth; x++) {
      const auto &cell_type = grid[viewTop + y][viewLeft + x];
      const auto &[ch, color] = cellTypeToCharColor[cell_type];
      attron(COLOR_PAIR(color));
      mvaddch(y, x, ch);
      attroff(COLOR_PAIR(color));
    }
  }
}

void GameBoardRenderer::drawFightInfo() {
  // implementation for drawing fight information
  // Here, ensure that the height is limited to termHeight
  // and width is limited to fightInfoWidth
  // The fight info should start from x-coordinate = boardWidth
}

void GameBoardRenderer::drawStats() {
  int row = termHeight - 5; // start from the reserved space

  for (const auto &stat : data.stats) {
    if (row < termHeight) { // Check if we have more space in terminal
      mvprintw(row++, 0, "%s: %s", stat.first.c_str(), stat.second.c_str());
    }
  }
}
