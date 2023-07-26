#ifndef GAME_BOARD_RENDERER_H
#define GAME_BOARD_RENDERER_H

#include "renderer_data.h"
#include "state_renderer.h"

enum class ColorPair { EMPTY = 1, WALL, PLAYER, GOBLIN, ORC, DRAGON, TROLL };
extern std::unordered_map<CellType, std::pair<char, ColorPair>>
    cellTypeToCharColor;

// Helper struct to hold the coordinates
struct Rect {
  double top;
  double right;
  double bottom;
  double left;
  Rect() : top(0), right(0), bottom(0), left(0) {}
  Rect(double top, double right, double bottom, double left)
      : top(top), right(right), bottom(bottom), left(left) {}
};

class GameBoardRenderer : public StateRenderer {
public:
  GameBoardRenderer(const RendererData &_data);
  ~GameBoardRenderer() override;

  void draw() override;

private:
  const RendererData
      &data; // Store a reference to the data needed for rendering

  // Components' sizes
  Rect boardRect;
  Rect messageDisplayRect;
  Rect statsRect;

  // Terminal size
  int termHeight;
  int termWidth;

  void drawBoard();
  void drawMessageDisplay();
  void drawStats();
};

#endif // GAME_BOARD_RENDERER_H
