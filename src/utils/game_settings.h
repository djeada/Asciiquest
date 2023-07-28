#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

enum class CellType {
  EMPTY = 0,
  WALL,
  PLAYER,
  GOBLIN,
  ORC,
  TROLL,
  DRAGON,
  START,
  END

};

enum class GameState { MAIN_MENU, GAMEPLAY, PAUSE_MENU, GAME_OVER };

#endif
