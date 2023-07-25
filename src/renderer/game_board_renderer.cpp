#include "game_board_renderer.h"
#include <ncurses.h>

std::unordered_map<CellType, std::pair<char, int>> cellTypeToCharColor = {
    {CellType::EMPTY, {' ', 1}},
    {CellType::WALL, {'#', 2}},
    {CellType::PLAYER, {'@', 3}},
    {CellType::GOBLIN, {'g', 4}},
};

GameBoardRenderer::GameBoardRenderer(const RendererData &_data) : data(_data) {
  start_color(); // Start color functionality

  // Define color pairs
  if (!has_colors()) {
    return;
  }

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);

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
  const auto &playerPosition = data.playerPosition;
  int boardHeight =
      std::min(termHeight - 5,
               static_cast<int>(grid.size())); // reserve 5 lines for stats
  int boardWidth = std::min(this->boardWidth, static_cast<int>(grid[0].size()));

  // Get terminal size
  getmaxyx(stdscr, termHeight, termWidth);

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
std::vector<std::string> splitStringToLines(const std::string &str,
                                            int lineWidth) {
  std::vector<std::string> result;
  int length = str.length();
  int start = 0;

  while (start < length) {
    int end = std::min(start + lineWidth, length);

    if (end != length && str[end] != ' ') {
      // If we're not at the end of the string, backtrack to the last space
      int lastSpace = str.rfind(' ', end);
      end = (lastSpace != std::string::npos) ? lastSpace : end;
    }

    // Push the line to the result
    result.push_back(str.substr(start, end - start));

    // Update start for the next iteration
    start = end;

    // Skip spaces at the start of the next line
    while (start < length && str[start] == ' ') {
      ++start;
    }
  }

  return result;
}

void GameBoardRenderer::drawFightInfo() {
  const auto &grid = data.grid;
  int x = std::min(this->boardWidth, static_cast<int>(grid[0].size()));
  int y = 0;

  for (const auto &info : data.fightInfo) {
    auto lines =
        splitStringToLines(info, COLS - x); // Split the info string into lines
    for (const auto &line : lines) {
      mvprintw(y++, x, "%s", line.c_str()); // Print each line separately
      // Prevent overflow if there are more fight info lines than screen rows
      if (y >= LINES) {
        break;
      }
    }
  }
}

void GameBoardRenderer::drawStats() {
  int row = termHeight - 5; // start from the reserved space

  for (const auto &stat : data.stats) {
    if (row < termHeight) { // Check if we have more space in terminal
      mvprintw(row++, 0, "%s: %s", stat.first.c_str(), stat.second.c_str());
    }
  }
}
