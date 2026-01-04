#ifndef MODEL_H
#define MODEL_H

#include "entities/monster.h"
#include "entities/movable_object.h"
#include "entities/player.h"
#include "entities/treasure.h"
#include "map.h"
#include "spell/spell_effect.h"
#include "utils/direction.h"
#include "utils/info_deque.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class Model {

public:
  Model();
  void update();

  void queuePlayerMove(const Point &point);
  void castPlayerSpell(int spellIndex, const Point &direction);
  void restart();
  bool isGameOver();
  std::unordered_map<std::string, std::string> getPlayerStats();

  std::shared_ptr<Player> player;
  std::shared_ptr<InfoDeque> info;
  std::shared_ptr<Map> map;
  std::vector<std::shared_ptr<Monster>> monsters;
  std::unordered_map<Point, std::shared_ptr<Treasure>> treasures;
  std::unordered_map<Point, std::shared_ptr<MovableObject>> movableObjects;
  std::vector<std::shared_ptr<SpellEffect>> activeSpellEffects;

  // Game progression
  int currentLevel;
  int monstersKilled;
  int totalScore;

private:
  void loadMap();
  void fight(const std::shared_ptr<Monster> &monster);
  void exploreTreasure(const std::shared_ptr<Treasure> &treasure);
  void spawnMonsters();
  int getDifficultyMultiplier() const;
  
  void updateSpellEffects();
  void checkSpellCollisions(const std::shared_ptr<SpellEffect> &effect);

  void attemptPlayerMove(const std::shared_ptr<Player> &player,
                         const Point &direction);
  void attemptMonsterMove(const std::shared_ptr<Monster> &monster,
                          const Point &direction);
  void updateEntityPosition(const std::shared_ptr<Entity> &entity,
                            const Point &oldPos, const Point &newPos);
  bool isWall(const Point &point);
  bool isPlayer(const Point &point);
  bool isExit(const Point &point);
  bool isMonster(const Point &point);
  bool isTreasure(const Point &point);
  bool isMovableObject(const Point &point);
  bool tryPushObject(const Point &objectPos, const Point &direction);
  std::atomic_bool running;
  std::queue<Point> playerMoves;
  std::chrono::steady_clock::time_point lastUpdate;
};

#endif // MODEL_H
