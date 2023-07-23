#include "state_renderer.h"

class GameBoardRenderer : public StateRenderer {
public:
  GameBoardRenderer();
  ~GameBoardRenderer();

  void draw(const std::vector<std::vector<CellType>> &grid,
            const std::vector<std::string> &fightInfo,
            const std::unordered_map<std::string, std::string> &stats);

private:
  void drawBoard(const std::vector<std::vector<CellType>> &grid);
  void drawFightInfo(const std::vector<std::string> &info);
  void drawStats(const std::unordered_map<std::string, std::string> &info);
};
