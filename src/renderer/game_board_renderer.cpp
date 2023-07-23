#include "game_board_renderer.h"
#include <ncurses.h>

std::unordered_map<CellType, std::pair<char, int>> cellTypeToCharColor = {
    {CellType::EMPTY, {' ', COLOR_BLACK}},
    {CellType::WALL, {'#', COLOR_BLUE}},
    {CellType::PLAYER, {'@', COLOR_RED}}

};

GameBoardRenderer::GameBoardRenderer() {}

GameBoardRenderer::~GameBoardRenderer() {}

void GameBoardRenderer::draw(
    const std::vector<std::vector<CellType>> &grid,
    const std::vector<std::string> &fightInfo,
    const std::unordered_map<std::string, std::string> &stats) {
  clear();
  drawBoard(grid);
  drawFightInfo(fightInfo);
  drawStats(stats);
}

void GameBoardRenderer::drawBoard(
    const std::vector<std::vector<CellType>> &grid) {
  for (int y = 0; y < grid.size(); y++) {
    for (int x = 0; x < grid[y].size(); x++) {
      auto cell_type = grid[y][x];
      const auto &[ch, color] = cellTypeToCharColor[cell_type];
      attron(COLOR_PAIR(color));
      mvaddch(y, x, ch);
      attroff(COLOR_PAIR(color));
    }
  }
  refresh();
}

void GameBoardRenderer::drawFightInfo(const std::vector<std::string> &info) {}
void GameBoardRenderer::drawStats(
    const std::unordered_map<std::string, std::string> &info) {}
