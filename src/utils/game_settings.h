#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

enum class CellType {
  START,
  END,
  EMPTY,
  WALL,
  PLAYER,
  GOBLIN,
  ORC,
  TROLL,
  DRAGON
};

enum class GameState { MAIN_MENU, GAMEPLAY, PAUSE_MENU, GAME_OVER };

#endif
