#ifndef RENDERER_H
#define RENDERER_H

#include "renderer_data.h"
#include "state_renderer.h"
#include "utils/game_settings.h"
#include <functional>
#include <map>
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

  void draw(const RendererData &data);
  void setState(GameState gameState);

private:
  GameState currentGameState;
  std::map<GameState,
           std::function<std::unique_ptr<StateRenderer>(const RendererData &)>>
      stateRendererMap;
};

#endif // RENDERER_H
