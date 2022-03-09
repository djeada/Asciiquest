#include "map.h"
#include "monster.h"

class Game {
  int level;
  Player player;
  Map *map;
  std::vector<Monster *> monsters;
  void initalizeMonsters(int count);

public:
  Game();
  ~Game();

  void run();
  void init();
  void updatePositions();
  void render();
  void fight(Entity &attacker, Entity &defender);
  void handleInput();
  void loadLevel();
  bool isGameOver();
  bool isLevelComplete();
  void gameOver();
};
