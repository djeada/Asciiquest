#include "game_board_renderer.h"
#include "model/spell/spell_effect.h"
#include "model/entities/trap.h"
#include "utils/global_config.h"
#include <algorithm>
#include <ncurses.h>

namespace {
struct PanelLayout {
  int top;
  int left;
  int bottom;
  int right;

  int width() const { return right - left; }
  int height() const { return bottom - top; }
};

PanelLayout getPanelLayout(const Rect &rect, int termHeight, int termWidth) {
  PanelLayout layout{};
  layout.left = std::max(0, std::min(termWidth - 1,
                                     static_cast<int>(rect.right * termWidth)));
  layout.top = std::max(0, std::min(termHeight - 1,
                                    static_cast<int>(rect.top * termHeight)));
  layout.right = std::max(layout.left + 1,
                          std::min(termWidth,
                                   static_cast<int>(rect.left * termWidth)));
  layout.bottom = std::max(layout.top + 1,
                           std::min(termHeight,
                                    static_cast<int>(rect.bottom * termHeight)));
  return layout;
}

void drawPanel(const PanelLayout &layout, const std::string &title,
               int borderColor, int titleColor) {
  if (layout.width() < 3 || layout.height() < 3) {
    return;
  }

  attron(COLOR_PAIR(borderColor));
  mvaddch(layout.top, layout.left, ACS_ULCORNER);
  mvaddch(layout.top, layout.right - 1, ACS_URCORNER);
  mvaddch(layout.bottom - 1, layout.left, ACS_LLCORNER);
  mvaddch(layout.bottom - 1, layout.right - 1, ACS_LRCORNER);

  mvhline(layout.top, layout.left + 1, ACS_HLINE, layout.width() - 2);
  mvhline(layout.bottom - 1, layout.left + 1, ACS_HLINE, layout.width() - 2);
  mvvline(layout.top + 1, layout.left, ACS_VLINE, layout.height() - 2);
  mvvline(layout.top + 1, layout.right - 1, ACS_VLINE, layout.height() - 2);
  attroff(COLOR_PAIR(borderColor));

  if (!title.empty() && layout.width() > 4) {
    int titleX = layout.left + 2;
    int maxTitleWidth = layout.width() - 4;
    std::string clippedTitle = title.substr(0, maxTitleWidth);
    attron(A_BOLD | COLOR_PAIR(titleColor));
    mvprintw(layout.top, titleX, "%s", clippedTitle.c_str());
    attroff(A_BOLD | COLOR_PAIR(titleColor));
  }
}

std::vector<std::string> wrapLines(const std::string &text, int width) {
  std::vector<std::string> result;
  if (width <= 0) {
    return result;
  }

  int length = static_cast<int>(text.length());
  int start = 0;

  while (start < length) {
    int end = std::min(start + width, length);
    if (end != length && text[end] != ' ') {
      int lastSpace = text.rfind(' ', end);
      end = (lastSpace != std::string::npos && lastSpace > start) ? lastSpace
                                                                  : end;
    }
    result.push_back(text.substr(start, end - start));
    start = end;
    while (start < length && text[start] == ' ') {
      ++start;
    }
  }

  return result;
}

int messageColor(MessageType type) {
  switch (type) {
  case MessageType::SYSTEM:
    return static_cast<int>(ColorPair::LOG_SYSTEM);
  case MessageType::COMBAT:
    return static_cast<int>(ColorPair::LOG_COMBAT);
  case MessageType::LOOT:
    return static_cast<int>(ColorPair::LOG_LOOT);
  case MessageType::INFO:
  default:
    return static_cast<int>(ColorPair::LOG_INFO);
  }
}

std::string messagePrefix(const MessageEntry &entry) {
  std::string tag;
  switch (entry.type) {
  case MessageType::SYSTEM:
    tag = "SYS";
    break;
  case MessageType::COMBAT:
    tag = "C";
    break;
  case MessageType::LOOT:
    tag = "L";
    break;
  case MessageType::INFO:
  default:
    tag = "I";
    break;
  }

  if (!entry.hasSource) {
    return "[" + tag + "] ";
  }
  return "[" + tag + " " + std::to_string(entry.source.x) + "," +
         std::to_string(entry.source.y) + "] ";
}
} // namespace

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
    {CellType::SKELETON,
     {GlobalConfig::getInstance().getConfig<char>("SkeletonSymbol"),
      ColorPair::SKELETON}},
    {CellType::START,
     {GlobalConfig::getInstance().getConfig<char>("StartSymbol"),
      ColorPair::START}},
    {CellType::END,
     {GlobalConfig::getInstance().getConfig<char>("EndSymbol"),
      ColorPair::END}},
    {CellType::TREASURE,
     {GlobalConfig::getInstance().getConfig<char>("TreasureSymbol"),
      ColorPair::TREASURE}},
    {CellType::POTION,
     {GlobalConfig::getInstance().getConfig<char>("PotionSymbol"),
      ColorPair::POTION}},
    {CellType::FIRE_PROJECTILE, {'*', ColorPair::FIRE_PROJECTILE}},
    {CellType::ICE_PROJECTILE, {'|', ColorPair::ICE_PROJECTILE}},
    {CellType::LIGHTNING_PROJECTILE, {'-', ColorPair::LIGHTNING_PROJECTILE}},
    {CellType::HEAL_EFFECT, {'+', ColorPair::HEAL_EFFECT}},
    {CellType::SHIELD_EFFECT, {'#', ColorPair::SHIELD_EFFECT}},
    {CellType::BOULDER, {'O', ColorPair::BOULDER}},
    {CellType::CRATE, {'=', ColorPair::CRATE}},
    {CellType::BARREL, {'%', ColorPair::BARREL}},
    {CellType::MOUNTAIN, {'^', ColorPair::MOUNTAIN}},
    {CellType::GRASS, {'&', ColorPair::GRASS}},
    {CellType::TREE, {'"', ColorPair::TREE}},
    {CellType::WATER, {'~', ColorPair::WATER}},
    {CellType::DESERT, {'.', ColorPair::DESERT}},
    {CellType::BLADE_TRAP, {'/', ColorPair::BLADE_TRAP}},
    {CellType::SPIKE_TRAP, {'^', ColorPair::SPIKE_TRAP}},
    {CellType::ARROW_TRAP, {'>', ColorPair::ARROW_TRAP}},
    {CellType::BLADE_PROJECTILE, {'s', ColorPair::BLADE_PROJECTILE}},
    {CellType::SPIKE_PROJECTILE, {'^', ColorPair::SPIKE_PROJECTILE}},
    {CellType::ARROW_PROJECTILE, {'o', ColorPair::ARROW_PROJECTILE}},
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
      {ColorPair::SKELETON, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::START, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::END, {COLOR_RED, COLOR_WHITE}},
      {ColorPair::TREASURE, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::POTION, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::FIRE_PROJECTILE, {COLOR_RED, COLOR_BLACK}},
      {ColorPair::ICE_PROJECTILE, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::LIGHTNING_PROJECTILE, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::HEAL_EFFECT, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::SHIELD_EFFECT, {COLOR_BLUE, COLOR_BLACK}},
      {ColorPair::BOULDER, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::CRATE, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::BARREL, {COLOR_MAGENTA, COLOR_BLACK}},
      {ColorPair::MOUNTAIN, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::GRASS, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::TREE, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::WATER, {COLOR_BLUE, COLOR_BLACK}},
      {ColorPair::DESERT, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::BLADE_TRAP, {COLOR_RED, COLOR_BLACK}},
      {ColorPair::SPIKE_TRAP, {COLOR_MAGENTA, COLOR_BLACK}},
      {ColorPair::ARROW_TRAP, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::BLADE_PROJECTILE, {COLOR_RED, COLOR_BLACK}},
      {ColorPair::SPIKE_PROJECTILE, {COLOR_MAGENTA, COLOR_BLACK}},
      {ColorPair::ARROW_PROJECTILE, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::UI_BORDER, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::UI_TITLE, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::UI_TEXT, {COLOR_WHITE, COLOR_BLACK}},
      {ColorPair::UI_ACCENT, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::UI_HP_BAR, {COLOR_GREEN, COLOR_BLACK}},
      {ColorPair::UI_MANA_BAR, {COLOR_BLUE, COLOR_BLACK}},
      {ColorPair::UI_XP_BAR, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::LOG_SYSTEM, {COLOR_CYAN, COLOR_BLACK}},
      {ColorPair::LOG_COMBAT, {COLOR_RED, COLOR_BLACK}},
      {ColorPair::LOG_LOOT, {COLOR_YELLOW, COLOR_BLACK}},
      {ColorPair::LOG_INFO, {COLOR_WHITE, COLOR_BLACK}},
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
  drawContent();
  doupdate(); // Apply all changes at once for double buffering
}

void GameBoardRenderer::drawContent() {
  drawBoard();
  drawMessageDisplay();
  drawStats();
  wnoutrefresh(stdscr); // Update virtual screen
}

void GameBoardRenderer::drawBoard() {

  getmaxyx(stdscr, termHeight, termWidth);

  PanelLayout boardPanel = getPanelLayout(boardRect, termHeight, termWidth);
  if (boardPanel.width() < 3 || boardPanel.height() < 3) {
    return;
  }
  drawPanel(boardPanel, " DUNGEON ", static_cast<int>(ColorPair::UI_BORDER),
            static_cast<int>(ColorPair::UI_TITLE));

  // Calculate board dimensions based on terminal size and grid size
  int gridRowSize = static_cast<int>(data.grid.size());
  int gridColSize = static_cast<int>(data.grid[0].size());
  int boardHeight =
      std::min(boardPanel.height() - 2, gridRowSize);
  int boardWidth =
      std::min(boardPanel.width() - 2, gridColSize);

  if (boardHeight <= 0 || boardWidth <= 0) {
    return;
  }

  // Determine the top and left view based on the player position
  int viewTop = std::max(0, std::min(gridRowSize - boardHeight,
                                     data.playerPosition.y - boardHeight / 2));
  int viewLeft = std::max(0, std::min(gridColSize - boardWidth,
                                      data.playerPosition.x - boardWidth / 2));

  // Render the game board based on the determined view
  for (int y = 0; y < boardHeight; ++y) {
    for (int x = 0; x < boardWidth; ++x) {
      // Fetch the character and color representation of the cell type
      const auto cellType = data.grid[viewTop + y][viewLeft + x];
      const auto &[ch, color] = cellTypeToCharColor[cellType];

      // Set color attribute, print the character and unset color attribute
      attron(COLOR_PAIR(static_cast<int>(color)));
      mvaddch(boardPanel.top + 1 + y, boardPanel.left + 1 + x, ch);
      attroff(COLOR_PAIR(static_cast<int>(color)));
    }
  }
  
  // Render spell effects on top of the board
  if (data.spellEffects != nullptr) {
    for (const auto &effect : *data.spellEffects) {
      auto frames = effect->getCurrentFrames();
      for (const auto &frame : frames) {
        // Check if the frame is within the visible area
        int screenX = frame.position.x - viewLeft;
        int screenY = frame.position.y - viewTop;
        
        if (screenX >= 0 && screenX < boardWidth && 
            screenY >= 0 && screenY < boardHeight) {
          
          const auto &[ch, color] = cellTypeToCharColor[frame.cellType];
          attron(COLOR_PAIR(static_cast<int>(color)));
          mvaddch(boardPanel.top + 1 + screenY, boardPanel.left + 1 + screenX,
                  ch);
          attroff(COLOR_PAIR(static_cast<int>(color)));
        }
      }
    }
  }
  
  // Render trap projectiles on top of the board
  if (data.traps != nullptr) {
    for (const auto &trap : *data.traps) {
      const auto &projectiles = trap->getProjectiles();
      for (const auto &proj : projectiles) {
        if (!proj.active) continue;
        
        // Check if the projectile is within the visible area
        int screenX = proj.position.x - viewLeft;
        int screenY = proj.position.y - viewTop;
        
        if (screenX >= 0 && screenX < boardWidth && 
            screenY >= 0 && screenY < boardHeight) {
          
          const auto &[ch, color] = cellTypeToCharColor[proj.cellType];
          attron(COLOR_PAIR(static_cast<int>(color)));
          mvaddch(boardPanel.top + 1 + screenY, boardPanel.left + 1 + screenX,
                  ch);
          attroff(COLOR_PAIR(static_cast<int>(color)));
        }
      }
    }
  }
}

void GameBoardRenderer::drawMessageDisplay() {
  getmaxyx(stdscr, termHeight, termWidth);

  PanelLayout logPanel =
      getPanelLayout(messageDisplayRect, termHeight, termWidth);
  PanelLayout statsPanel = getPanelLayout(statsRect, termHeight, termWidth);
  if (statsPanel.top > logPanel.top && statsPanel.top < logPanel.bottom) {
    logPanel.bottom = statsPanel.top;
  }
  if (logPanel.width() < 3 || logPanel.height() < 3) {
    return;
  }
  drawPanel(logPanel, " LOG ", static_cast<int>(ColorPair::UI_BORDER),
            static_cast<int>(ColorPair::UI_TITLE));

  int x = logPanel.left + 1;
  int y = logPanel.top + 1;
  int width = logPanel.width() - 2;
  int height = logPanel.height() - 2;

  if (width <= 0 || height <= 0) {
    return;
  }

  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
  mvprintw(y++, x, " I/K scroll | Tags: C combat, L loot, SYS system");
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));

  for (const auto &entry : data.messageQueue.reverse()) {
    if (y >= logPanel.top + height) {
      return;
    }

    std::string prefix = messagePrefix(entry);
    int prefixWidth = static_cast<int>(prefix.size());
    int availableWidth = width - prefixWidth;
    if (availableWidth <= 0) {
      return;
    }

    int colorPair = messageColor(entry.type);
    attron(COLOR_PAIR(colorPair));

    for (size_t lineIndex = 0; lineIndex < entry.lines.size(); ++lineIndex) {
      if (y >= logPanel.top + height) {
        attroff(COLOR_PAIR(colorPair));
        return;
      }

      std::string text = entry.lines[lineIndex];
      if (lineIndex == entry.lines.size() - 1 && entry.repeatCount > 1) {
        text += " x" + std::to_string(entry.repeatCount);
      }

      auto wrapped = wrapLines(text, availableWidth);
      for (size_t i = 0; i < wrapped.size(); ++i) {
        if (y >= logPanel.top + height) {
          attroff(COLOR_PAIR(colorPair));
          return;
        }
        std::string linePrefix = (i == 0) ? prefix : std::string(prefixWidth, ' ');
        mvprintw(y++, x, "%s%s", linePrefix.c_str(), wrapped[i].c_str());
      }
    }

    attroff(COLOR_PAIR(colorPair));
    if (y < logPanel.top + height) {
      mvprintw(y++, x, " ");
    }
  }
}

void GameBoardRenderer::drawStats() {
  getmaxyx(stdscr, termHeight, termWidth);

  PanelLayout statsPanel = getPanelLayout(statsRect, termHeight, termWidth);
  if (statsPanel.width() < 3 || statsPanel.height() < 3) {
    return;
  }
  drawPanel(statsPanel, " STATUS ", static_cast<int>(ColorPair::UI_BORDER),
            static_cast<int>(ColorPair::UI_TITLE));

  int xStart = statsPanel.left + 1;
  int yStart = statsPanel.top + 1;
  int y = yStart;
  int contentWidth = statsPanel.width() - 2;
  int contentHeight = statsPanel.height() - 2;

  int maxBarWidth = std::max(10, contentWidth - 12);
  int labelWidth = 6;

  auto drawProgressBar = [&](int barY, const std::string &label,
                             float percentage, int color, int current,
                             int maximum) {
    // draw label
    mvprintw(barY, xStart, "%-*s", labelWidth,
             (label + ":").c_str());

    // draw the progress bar
    int progressBarWidth = std::max(4, maxBarWidth - labelWidth - 8);
    int progress = static_cast<int>(progressBarWidth * percentage);

    // set color and draw progress
    attron(COLOR_PAIR(color));
    for (int i = 0; i < progress; i++) {
      mvaddch(barY, xStart + labelWidth + 1 + i, '=');
    }
    attroff(COLOR_PAIR(color));

    attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
    for (int i = progress; i < progressBarWidth; i++) {
      mvaddch(barY, xStart + labelWidth + 1 + i, '-');
    }
    mvprintw(barY, xStart + labelWidth + 1 + progressBarWidth + 1, "%d/%d",
             current, maximum);
    attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_TEXT)));
  };

  // === GAME STATS ===
  attron(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::UI_TITLE)));
  mvprintw(y++, xStart, " DUNGEON LV.%s", data.stats["DungeonLevel"].c_str());
  attroff(A_BOLD | COLOR_PAIR(static_cast<int>(ColorPair::UI_TITLE)));

  // Print Character Level and Strength
  mvprintw(y++, xStart, " Char Lv: %s  STR: %s",
           data.stats["Level"].c_str(),
           data.stats["Strength"].c_str());

  // Render Health
  int health = std::stoi(data.stats["Health"]);
  int maxHealth = std::stoi(data.stats["MaxHealth"]);
  float healthPercentage =
      maxHealth > 0 ? static_cast<float>(health) / maxHealth : 0.0f;
  drawProgressBar(y++, "HP", healthPercentage,
                  static_cast<int>(ColorPair::UI_HP_BAR), health, maxHealth);

  // Render Mana
  int mana = std::stoi(data.stats["Mana"]);
  int maxMana = std::stoi(data.stats["MaxMana"]);
  float manaPercentage =
      maxMana > 0 ? static_cast<float>(mana) / maxMana : 0.0f;
  drawProgressBar(y++, "MP", manaPercentage,
                  static_cast<int>(ColorPair::UI_MANA_BAR), mana, maxMana);

  // Render Experience
  int exp = std::stoi(data.stats["Experience"]);
  int maxExp = std::stoi(data.stats["MaxExp"]);
  float expPercentage =
      maxExp > 0 ? static_cast<float>(exp) / maxExp : 0.0f;
  drawProgressBar(y++, "XP", expPercentage,
                  static_cast<int>(ColorPair::UI_XP_BAR), exp, maxExp);

  y++; // Empty line

  // === PROGRESS STATS ===
  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));
  mvprintw(y++, xStart, " Score: %s", data.stats["Score"].c_str());
  mvprintw(y++, xStart, " Kills: %s", data.stats["MonstersKilled"].c_str());
  mvprintw(y++, xStart, " Enemies: %s", data.stats["MonstersRemaining"].c_str());
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));

  y++; // Empty line

  // === LOCATION INFO ===
  attron(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));
  mvprintw(y++, xStart, " X: %d, Y: %d", data.playerPosition.x,
           data.playerPosition.y);
  
  // Get map dimensions from grid
  int mapHeight = static_cast<int>(data.grid.size());
  int mapWidth = data.grid.empty() ? 0 : static_cast<int>(data.grid[0].size());
  mvprintw(y++, xStart, " Map: %d x %d", mapWidth, mapHeight);
  attroff(COLOR_PAIR(static_cast<int>(ColorPair::UI_ACCENT)));

  y++; // Empty line

  if (y < yStart + contentHeight) {
    std::string controls =
        "Move: WASD/Arrows  Spells: 1-5  Pause: P  Quit: Q/ESC";
    for (const auto &line : wrapLines(controls, contentWidth - 1)) {
      if (y >= yStart + contentHeight) {
        break;
      }
      mvprintw(y++, xStart, "%s", line.c_str());
    }
  }
}
