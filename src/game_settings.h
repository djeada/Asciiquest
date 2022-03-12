#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_GAME_SETTINGS_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_GAME_SETTINGS_H

struct Symbols {
  static const char player = '@';
  static const char deadPlayer = 'x';
  static const char wall = '#';
  static const char emptySpace = ' ';
  static const char goblin = 'g';
  static const char orc = 'o';
  static const char dragon = 'd';
  static const char troll = 't';
  static const char treasure = '*';
};

struct Colors {
  static const int player = 1;
  static const int monster = 2;
  static const int goblin = 3;
  static const int orc = 4;
  static const int troll = 5;
  static const int dragon = 6;
  static const int treasure = 7;
};

struct GameSettings {
  static const int playerHealth = 1000;
  static const int playerDamage = 10;
  static const int monsterCount = 10;
  static const int goblinHealth = 5;
  static const int orcHealth = 10;
  static const int trollHealth = 15;
  static const int dragonHealth = 20;
  static const int goblinDamage = 5;
  static const int orcDamage = 10;
  static const int trollDamage = 15;
  static const int dragonDamage = 20;
  static const int goblinExp = 5;
  static const int orcExp = 10;
  static const int trollExp = 15;
  static const int dragonExp = 20;
  static const int maxWidth = 80;
  static const int maxHeight = 24;
  static const int horizontalOffset = 25;
  static const int verticalOffset = 1;
};

#endif
