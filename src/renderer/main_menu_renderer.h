#include "state_renderer.h"

class MainMenuRenderer : public StateRenderer {
public:
  MainMenuRenderer();
  ~MainMenuRenderer();

  void draw(const std::vector<std::vector<CellType>> &grid,
            const std::vector<std::string> &fightInfo,
            const std::unordered_map<std::string, std::string> &stats);
};
