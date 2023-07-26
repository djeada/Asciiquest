#include "game_board_renderer.h"
#include "utils/global_config.h"
#include <ncurses.h>

enum class ColorPair { EMPTY = 1, WALL, PLAYER, GOBLIN };

std::unordered_map<CellType, std::pair<char, ColorPair>> cellTypeToCharColor = {
    {CellType::EMPTY, {' ', ColorPair::EMPTY}},
    {CellType::WALL, {'#', ColorPair::WALL}},
    {CellType::PLAYER, {'@', ColorPair::PLAYER}},
    {CellType::GOBLIN, {'g', ColorPair::GOBLIN}},
};

GameBoardRenderer::GameBoardRenderer(const RendererData &_data) : data(_data) {
  start_color(); // Start color functionality

  // Define color pairs
  init_pair(static_cast<int>(ColorPair::EMPTY), COLOR_WHITE, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::WALL), COLOR_BLUE, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::PLAYER), COLOR_RED, COLOR_BLACK);
  init_pair(static_cast<int>(ColorPair::GOBLIN), COLOR_GREEN, COLOR_BLACK);

  // Rectangels holding ratios
  auto getConfigRect = [](const std::string &leftKey, const std::string &topKey,
                          const std::string &bottomKey,
                          const std::string &rightKey) {
    return Rect{GlobalConfig::getInstance().getConfig<double>(leftKey),
                GlobalConfig::getInstance().getConfig<double>(topKey),
                GlobalConfig::getInstance().getConfig<double>(bottomKey),
                GlobalConfig::getInstance().getConfig<double>(rightKey)};
  };

  boardRect = getConfigRect("BoardRectLeft", "BoardRectTop", "BoardRectBottom",
                            "BoardRectRight");
  messageDisplayRect =
      getConfigRect("MessageDisplayRectLeft", "MessageDisplayRectTop",
                    "MessageDisplayRectBottom", "MessageDisplayRectRight");
  statsRect = getConfigRect("StatsRectLeft", "StatsRectTop", "StatsRectBottom",
                            "StatsRectRight");
}

GameBoardRenderer::~GameBoardRenderer() {}

void GameBoardRenderer::draw() {
  clear();
  drawBoard();
  drawMessageDisplay();
  drawStats();
  refresh();
}

void GameBoardRenderer::drawBoard() {

  getmaxyx(stdscr, termHeight, termWidth);

  // Calculate board dimensions based on terminal size and grid size
  int gridRowSize = static_cast<int>(data.grid.size());
  int gridColSize = static_cast<int>(data.grid[0].size());
  int boardHeight =
      std::min(static_cast<int>(boardRect.bottom * termHeight), gridRowSize);
  int boardWidth =
      std::min(static_cast<int>(boardRect.left * termWidth), gridColSize);

  // Determine the top and left view based on the player position
  int viewTop = std::max(static_cast<int>(boardRect.top * termHeight),
                         std::min(gridRowSize - boardHeight,
                                  data.playerPosition.y - boardHeight / 2));
  int viewLeft = std::max(static_cast<int>(boardRect.right * termWidth),
                          std::min(gridColSize - boardWidth,
                                   data.playerPosition.x - boardWidth / 2));

  // Render the game board based on the determined view
  for (int y = 0; y < boardHeight; ++y) {
    for (int x = 0; x < boardWidth; ++x) {
      // Fetch the character and color representation of the cell type
      const auto cell_type = data.grid[viewTop + y][viewLeft + x];
      const auto &[ch, color] = cellTypeToCharColor[cell_type];

      // Set color attribute, print the character and unset color attribute
      attron(COLOR_PAIR(static_cast<int>(color)));
      mvaddch(y, x, ch);
      attroff(COLOR_PAIR(static_cast<int>(color)));
    }
  }
}

void GameBoardRenderer::drawMessageDisplay() {
  auto splitStringToLines = [](const std::string &str, int lineWidth) {
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
  };

  getmaxyx(stdscr, termHeight, termWidth);

  int x = std::min(static_cast<int>(messageDisplayRect.right * termWidth),
                   static_cast<int>(data.grid[0].size()));
  int y = static_cast<int>(messageDisplayRect.top * termHeight);

  int infoHeight =
      std::min(static_cast<int>(messageDisplayRect.bottom * termHeight), LINES);

  for (const auto &messgaes : data.fightInfo) {
    for (const auto &info : messgaes) {
      // Prevent overflow if there are more fight info lines than screen rows
      if (y >= infoHeight) {
        break;
      }
      auto lines = splitStringToLines(
          info, COLS - x - 1); // Subtract 1 to account for the empty space
      for (const auto &line : lines) {
        mvprintw(y++, x + 1, " %s",
                 line.c_str()); // line + empty space
        if (y >= infoHeight) {
          break;
        }
      }
    }
    mvprintw(y++, x, " \n");
  }
}

void GameBoardRenderer::drawStats() {

  getmaxyx(stdscr, termHeight, termWidth);
  int y = static_cast<int>(statsRect.top * termHeight);
  for (const auto &stat : data.stats) {
    if (y < termHeight) {
      mvprintw(y++, 0, "%s: %s", stat.first.c_str(), stat.second.c_str());
    }
  }
}
