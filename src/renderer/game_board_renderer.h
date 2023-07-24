#ifndef GAME_BOARD_RENDERER_H
#define GAME_BOARD_RENDERER_H

#include "renderer_data.h"
#include "state_renderer.h"

class GameBoardRenderer : public StateRenderer {
public:
  GameBoardRenderer(const RendererData &_data);
  ~GameBoardRenderer() override;

  void draw() override;

private:
  const RendererData
      &data; // Store a reference to the data needed for rendering

  void drawBoard();
  void drawFightInfo();
  void drawStats();

  // Terminal size
  int termHeight;
  int termWidth;

  // Components' sizes
  int boardWidth;
  int boardHeight;
  int fightInfoWidth;
  int fightInfoHeight;
  int statsHeight;
};

#endif // GAME_BOARD_RENDERER_H
