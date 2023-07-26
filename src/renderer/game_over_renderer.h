#ifndef GAME_OVER_RENDERER_H
#define GAME_OVER_RENDERER_H

#include "game_board_renderer.h"
#include <memory>

class GameOverRenderer : public StateRenderer {
public:
  GameOverRenderer(const RendererData &_data);
  ~GameOverRenderer() override;

  void draw() override;

private:
  const RendererData
      &data; // Store a reference to the data needed for rendering

  void drawGameOver();

  // Terminal size
  int termHeight;
  int termWidth;
};

#endif // GAME_OVER_RENDERER_H
