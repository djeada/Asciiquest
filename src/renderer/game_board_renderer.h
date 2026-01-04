#ifndef GAME_BOARD_RENDERER_H
#define GAME_BOARD_RENDERER_H

#include "renderer_data.h"
#include "state_renderer.h"

enum class ColorPair {
  EMPTY = 1,
  WALL,
  FLOOR,
  DOOR,
  PLAYER,
  GOBLIN,
  ORC,
  DRAGON,
  TROLL,
  SKELETON,
  TREASURE,
  POTION,
  START,
  END,
  FIRE_PROJECTILE,
  ICE_PROJECTILE,
  LIGHTNING_PROJECTILE,
  HEAL_EFFECT,
  SHIELD_EFFECT,
  BOULDER,
  CRATE,
  BARREL,
  MOUNTAIN,
  GRASS,
  TREE,
  WATER,
  DESERT,
  BLADE_TRAP,
  SPIKE_TRAP,
  ARROW_TRAP,
  BLADE_PROJECTILE,
  SPIKE_PROJECTILE,
  ARROW_PROJECTILE,
  UI_BORDER,
  UI_TITLE,
  UI_TEXT,
  UI_ACCENT,
  UI_HP_BAR,
  UI_MANA_BAR,
  UI_XP_BAR,
  LOG_SYSTEM,
  LOG_COMBAT,
  LOG_LOOT,
  LOG_INFO
};
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
  void drawContent(); // Draw content without finalizing (for composite renderers)
  void drawBoard();
  void drawMessageDisplay();
  void drawStats();

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
};

#endif // GAME_BOARD_RENDERER_H
