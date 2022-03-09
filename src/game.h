#include "map.h"
#include "monster.h"
#include <memory>

class Game {
  int level;
  Player player;
  Map *map;
  std::vector<std::unique_ptr<Monster>> monsters;
  void initalizeMonsters(int count);
  void updateEntityPosition(Entity &entity, int dx = 0, int dy = 0);

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
