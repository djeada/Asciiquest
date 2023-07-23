#include "utils/game_settings.h"
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>

class Renderer {
public:
  Renderer();
  ~Renderer();

  void drawBoard(const std::vector<std::vector<CellType>> &grid);
  void drawFightInfo(const std::vector<std::string> &info);
  void drawStats(const std::unordered_map<std::string, std::string> &info);
};
