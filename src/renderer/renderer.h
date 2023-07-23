#ifndef RENDERER_H
#define RENDERER_H

#include "state_renderer.h" // path to the StateRenderer class
#include "utils/game_settings.h"
#include <memory> // for unique_ptr
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>

class Renderer {
public:
  Renderer();
  Renderer(const Renderer &);

  ~Renderer();

  void setState(GameState gameState);
  void draw(const std::vector<std::vector<CellType>> &grid,
            const std::vector<std::string> &fightInfo,
            const std::unordered_map<std::string, std::string> &stats);

private:
  std::unique_ptr<StateRenderer> currentStateRenderer;
};

#endif // RENDERER_H
