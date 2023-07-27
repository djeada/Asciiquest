#include "game_board_renderer.h"
#include "utils/global_config.h"
#include <ncurses.h>

std::unordered_map<CellType, std::pair<char, ColorPair>> cellTypeToCharColor = {
    {CellType::EMPTY,
     {GlobalConfig::getInstance().getConfig<int>("EmptySymbol"),
      ColorPair::EMPTY}},
    {CellType::WALL,
     {GlobalConfig::getInstance().getConfig<char>("WallSymbol"),
      ColorPair::WALL}},
    {CellType::PLAYER,
     {GlobalConfig::getInstance().getConfig<char>("PlayerSymbol"),
      ColorPair::PLAYER}},
    {CellType::GOBLIN,
     {GlobalConfig::getInstance().getConfig<char>("GoblinSymbol"),
      ColorPair::GOBLIN}},
    {CellType::ORC,
     {GlobalConfig::getInstance().getConfig<char>("OrcSymbol"),
      ColorPair::ORC}},
    {CellType::DRAGON,
     {GlobalConfig::getInstance().getConfig<char>("DragonSymbol"),
      ColorPair::DRAGON}},
    {CellType::TROLL,
     {GlobalConfig::getInstance().getConfig<char>("TrollSymbol"),
      ColorPair::TROLL}},
    {CellType::START,
     {GlobalConfig::getInstance().getConfig<char>("StartSymbol"),
      ColorPair::START}},
    {CellType::END,
     {GlobalConfig::getInstance().getConfig<char>("EndSymbol"),
      ColorPair::END}},
};

GameBoardRenderer::GameBoardRenderer(const RendererData &_data) : data(_data) {
  start_color(); // Start color functionality

  // Define color pairs
  std::unordered_map<ColorPair, std::pair<int, int>> colorDefinitions = {
      {ColorPair::EMPTY, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::WALL, {COLOR_BLUE, COLOR_BLACK}},
      {ColorPair::PLAYER, {COLOR_RED, COLOR_BLACK}},
      {ColorPair::GOBLIN, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::ORC, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::DRAGON, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::TROLL, {COLOR_MAGENTA, COLOR_BLACK}},
      {ColorPair::START, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::END, {COLOR_RED, COLOR_WHITE}},
  };

  // Initialize color pairs
  for (const auto &pair : colorDefinitions) {
    init_pair(static_cast<int>(pair.first), pair.second.first,
              pair.second.second);
  }

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
      const auto cellType = data.grid[viewTop + y][viewLeft + x];
      const auto &[ch, color] = cellTypeToCharColor[cellType];

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

  for (const auto &messgaes : data.messageQueue.reverse()) {
    for (const auto &info : messgaes) {
      // Prevent overflow if there are more fight info lines than screen rows
      if (y >= infoHeight) {
        break;
      }
      auto lines = splitStringToLines(
          info, COLS - x - 3); // Subtract 2 to account for the empty space
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

  // calculate positions based on statsRect
  int yLevel = static_cast<int>(statsRect.top * termHeight) + 1;
  int yHealth = yLevel + 1;
  int yExp = yHealth + 1;

  // initialize colors
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);

  int maxBarWidth = termWidth / 2;
  int labelWidth = 8;

  auto drawProgressBar = [&](int y, const std::string &label, float percentage,
                             int color) {
    // draw label
    mvprintw(y, 0, "%-*s", labelWidth,
             (label + ": ")
                 .c_str()); // Left justify the label to a width of labelWidth

    // draw the progress bar
    int progressBarWidth = maxBarWidth - labelWidth - 2; // Adjust as needed
    int progress = static_cast<int>(progressBarWidth * percentage);

    // set color and draw progress
    attron(COLOR_PAIR(color));
    for (int i = 0; i < progress; i++) {
      mvprintw(y, labelWidth + i, "=");
    }
    attroff(COLOR_PAIR(color));
  };

  // Print Level
  mvprintw(yLevel, 0, " Level: %s", data.stats["Level"].c_str());

  // Render Health
  float healthPercentage =
      stof(data.stats["Health"]) / stof(data.stats["MaxHealth"]);
  drawProgressBar(yHealth, " Health", healthPercentage, 1); // 1 = COLOR_GREEN

  // Render Experience
  float expPercentage =
      stof(data.stats["Experience"]) / stof(data.stats["MaxExp"]);
  drawProgressBar(yExp, " Exp", expPercentage, 2); // 2 = COLOR_BLUE
}
