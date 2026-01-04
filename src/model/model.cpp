#include "model.h"
#include "utils/global_config.h"
#include <chrono>
#include <cmath>
#include <queue>

const int monsterUpdateSpeed =
    GlobalConfig::getInstance().getConfig<int>("MonsterUpdateSpeed");

namespace {
std::string formatCoords(const Point &pos) {
  return "[" + std::to_string(pos.x) + "," + std::to_string(pos.y) + "]";
}

template <typename T> std::string labelWithCoords(const T &entity) {
  return entity.toString() + " " + formatCoords(entity.position);
}
} // namespace

Model::Model()
    : running(false), lastUpdate(std::chrono::steady_clock::now()),
      currentLevel(0), monstersKilled(0), totalScore(0) {}

int Model::getDifficultyMultiplier() const {
  // Difficulty increases by 10% per level
  // Ensure minimum level of 1 to avoid weaker monsters
  int level = std::max(1, currentLevel);
  return static_cast<int>(100 * std::pow(1.1, level - 1));
}

void Model::spawnMonsters() {
  monsters.clear();

  auto addMonsters = [this](const std::string &type, auto monsterMaker) {
    int monsterCount = GlobalConfig::getInstance().getConfig<int>(type);
    // Scale monster count with level (up to 50% more monsters at higher levels)
    monsterCount = monsterCount + (monsterCount * (currentLevel - 1) / 10);
    monsters.reserve(monsters.size() + monsterCount);

    for (int i = 0; i < monsterCount; i++) {
      auto monster =
          std::make_shared<decltype(monsterMaker)>(monsterMaker);
      // Scale monster health and damage with difficulty
      int diffMult = getDifficultyMultiplier();
      monster->health = (monster->health * diffMult) / 100;
      monster->strength = (monster->strength * diffMult) / 100;
      monsters.push_back(monster);
    }
  };

  addMonsters("GoblinsCount", Goblin());
  addMonsters("TrollsCount", Troll());
  addMonsters("DragonsCount", Dragon());
  addMonsters("SkeletonsCount", Skeleton());

  // Adding Orcs separately as they have different parameters
  int orcsCount = GlobalConfig::getInstance().getConfig<int>("OrcsCount");
  orcsCount = orcsCount + (orcsCount * (currentLevel - 1) / 10);
  monsters.reserve(monsters.size() + orcsCount);
  for (int i = 0; i < orcsCount; i++) {
    auto orc = std::make_shared<Orc>(map, player);
    int diffMult = getDifficultyMultiplier();
    orc->health = (orc->health * diffMult) / 100;
    orc->strength = (orc->strength * diffMult) / 100;
    monsters.push_back(orc);
  }
}

void Model::restart() {
  // Track level progression
  if (!player || !player->isAlive()) {
    player = std::make_shared<Player>();
    currentLevel = 1;
    monstersKilled = 0;
    totalScore = 0;
  } else {
    // Advancing to next level
    currentLevel++;
    // Bonus score for completing level
    totalScore += currentLevel * 1000;
    // Heal player slightly when advancing
    player->heal(player->getMaxHealth() / 4);
  }

  map = std::make_shared<Map>(
      GlobalConfig::getInstance().getConfig<int>("MapWidth"),
      GlobalConfig::getInstance().getConfig<int>("MapHeight"));
  info = std::make_shared<InfoDeque>(
      GlobalConfig::getInstance().getConfig<int>("MessageQueueSize"));

  // Use new spawn system with difficulty scaling
  spawnMonsters();

  loadMap();
}

void Model::loadMap() {
  map->loadLevel();
  map->setCellType(map->getStart(), CellType::PLAYER);
  player->move(map->getStart());

  for (const auto &monster : monsters) {
    auto position = map->randomFreePosition();
    monster->position = position;
    map->setCellType(position, monster->cellType);
  }

  // Scale treasure count with level
  int treasureCount =
      GlobalConfig::getInstance().getConfig<int>("TreasureCount");
  treasureCount = treasureCount + (currentLevel * 2); // More treasures at higher levels
  treasures.clear();
  for (int i = 0; i < treasureCount; ++i) {
    auto treasurePtr = std::make_shared<Treasure>();
    auto position = map->randomFreePosition();
    treasurePtr->move(position);
    treasures.emplace(position, std::move(treasurePtr));
    map->setCellType(position, CellType::TREASURE);
  }

  map->setCellType(map->getEnd(), CellType::END);

  // Spawn movable objects
  movableObjects.clear();
  int objectsPerType = 2 + (currentLevel / 2); // More objects at higher levels
  
  // Add boulders
  for (int i = 0; i < objectsPerType; ++i) {
    auto position = map->randomFreePosition();
    auto boulder = std::make_shared<Boulder>(position);
    movableObjects.emplace(position, boulder);
    map->setCellType(position, CellType::BOULDER);
  }
  
  // Add crates
  for (int i = 0; i < objectsPerType; ++i) {
    auto position = map->randomFreePosition();
    auto crate = std::make_shared<Crate>(position);
    movableObjects.emplace(position, crate);
    map->setCellType(position, CellType::CRATE);
  }
  
  // Add barrels
  for (int i = 0; i < objectsPerType; ++i) {
    auto position = map->randomFreePosition();
    auto barrel = std::make_shared<Barrel>(position);
    movableObjects.emplace(position, barrel);
    map->setCellType(position, CellType::BARREL);
  }

  // Show level info
  info->addMessage(MessageType::SYSTEM, &player->position,
                   "Entering Dungeon Level " + std::to_string(currentLevel));
  info->addMessage(MessageType::SYSTEM, &player->position,
                   "Monsters: " + std::to_string(monsters.size()) +
                       " | Treasures: " + std::to_string(treasureCount));
  if (currentLevel > 1) {
    info->addMessage(MessageType::SYSTEM, &player->position,
                     "Difficulty increased. Monsters are stronger.");
  }
  info->addMessage(MessageType::SYSTEM, &player->position,
                   "Find the exit to advance.");
  
  // Spawn traps
  traps.clear();
  int trapCount = 2 + (currentLevel / 2); // More traps at higher levels
  
  // Add blade traps
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    auto trap = std::make_shared<BladeTrap>(position);
    traps.push_back(trap);
    map->setCellType(position, CellType::BLADE_TRAP);
  }
  
  // Add spike traps
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    auto trap = std::make_shared<SpikeTrap>(position);
    traps.push_back(trap);
    map->setCellType(position, CellType::SPIKE_TRAP);
  }
  
  // Add arrow traps (pointing in different directions)
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    Point direction = (i % 2 == 0) ? Direction::DOWN : Direction::RIGHT;
    auto trap = std::make_shared<ArrowTrap>(position, direction);
    traps.push_back(trap);
    map->setCellType(position, CellType::ARROW_TRAP);
  }
}

void Model::update() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

  while (!playerMoves.empty()) {
    auto offset = playerMoves.front();
    playerMoves.pop();
    attemptPlayerMove(player, offset);
  }
  
  // Update spell effects
  updateSpellEffects();
  
  // Update traps
  updateTraps();
  checkTrapCollisions();

  if (elapsed.count() < monsterUpdateSpeed) {
    return;
  }

  for (const auto &monster : monsters) {
    attemptMonsterMove(monster, monster->getVelocity());
  }

  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                [](const std::shared_ptr<Monster> &monster) {
                                  return !monster->isAlive();
                                }),
                 monsters.end());

  lastUpdate = now;
}

void Model::fight(const std::shared_ptr<Monster> &monster) {

  auto attack = [&](const std::shared_ptr<Entity> &attacker,
                    const std::shared_ptr<Entity> &defender) {
    double successRate = (rand() % 100) / 100.0; // random value between 0 and 1
    bool attackerIsPlayer = (attacker.get() == player.get());
    bool defenderIsPlayer = (defender.get() == player.get());

    std::string attackerLabel =
        attackerIsPlayer ? "You" : labelWithCoords(*attacker);
    std::string defenderLabel =
        defenderIsPlayer ? "you" : labelWithCoords(*defender);

    // 15% chance of attack missing
    if (successRate > 0.85) {
      info->addMessage(MessageType::COMBAT, &attacker->position,
                       attackerLabel + " miss " + defenderLabel + ".");
      return;
    }

    // 10% chance of attack being blocked
    if (successRate < 0.1) {
      std::string blockMessage =
          defenderIsPlayer
              ? "You block " +
                    (attackerIsPlayer ? "your own"
                                      : attackerLabel + "'s") +
                    " attack."
              : defenderLabel + " blocks " +
                    (attackerIsPlayer ? "your" : attackerLabel + "'s") +
                    " attack.";
      info->addMessage(MessageType::COMBAT, &defender->position,
                       std::move(blockMessage));
      return;
    }

    // Calculate base damage with randomness
    int damage = static_cast<int>(attacker->strength * successRate);

    // Critical hit system (10% chance for 2x damage)
    bool isCritical = (rand() % 100) < 10;
    if (isCritical) {
      damage *= 2;
      info->addMessage(MessageType::COMBAT, &attacker->position,
                       "Critical hit!");
    }

    defender->takeDamage(damage);
    info->addMessage(MessageType::COMBAT, &attacker->position,
                     attackerLabel + " hit " + defenderLabel + " for " +
                         std::to_string(damage) + ".");
  };

  auto updateMapAfterFight = [&](const auto &defeatedMonster) {
    if (!defeatedMonster->isAlive()) {
      map->setCellType(defeatedMonster->position, CellType::EMPTY);
    }

    if (!player->isAlive()) {
      map->setCellType(player->position, CellType::EMPTY);
    } else {
      map->setCellType(player->position, player->cellType);
    }
  };

  info->addMessage(MessageType::COMBAT, &player->position,
                   "Battle: You vs " + labelWithCoords(*monster));

  int round = 0;
  while (player->isAlive() && monster->isAlive()) {
    round++;
    info->addMessage(MessageType::COMBAT, &player->position,
                     "Round " + std::to_string(round));
    attack(player, monster);
    if (monster->isAlive()) {
      attack(monster, player);
    }
  }

  // Handle fight outcome
  if (!monster->isAlive()) {
    monstersKilled++;
    int expGain = monsterExpMap[monster->cellType];
    int scoreGain = expGain * currentLevel;
    totalScore += scoreGain;
    player->addExperience(expGain);
    info->addMessage(MessageType::COMBAT, &player->position,
                     "You defeated " + labelWithCoords(*monster) + ". +" +
                         std::to_string(expGain) + " EXP, +" +
                         std::to_string(scoreGain) + " Score");
  } else if (!player->isAlive()) {
    info->addMessage(MessageType::SYSTEM, &player->position, "Game Over");
    info->addMessage(MessageType::SYSTEM, &player->position,
                     "Final Score: " + std::to_string(totalScore));
    info->addMessage(MessageType::SYSTEM, &player->position,
                     "Monsters Killed: " + std::to_string(monstersKilled));
    info->addMessage(MessageType::SYSTEM, &player->position,
                     "Dungeon Level Reached: " +
                         std::to_string(currentLevel));
  }

  updateMapAfterFight(monster);
}

void Model::exploreTreasure(const std::shared_ptr<Treasure> &treasure) {

  // Initialize success rate (you might want to tweak the numbers depending on
  // your game balance)
  double successRate = (rand() % 100) / 100.0; // random value between 0 and 1

  // Define the mechanism of exploring treasure
  auto explore = [&](const auto &explorer, const auto &treasure,
                     auto &messages) {
    if (successRate > 0.85) { // 15% chance of exploration failure
      messages.push_back("You fail to loot " + labelWithCoords(*treasure) +
                         ".");
      return;
    }

    // Get treasure's bonus value
    int bonus = treasure->getValue() * successRate; // add randomness to bonus

    // Depending on the type of the bonus, apply it to the player
    switch (treasure->getBonusType()) {
    case BonusType::Experience:
      explorer->addExperience(bonus);
      break;
    case BonusType::Health:
      explorer->heal(bonus);
      break;
    case BonusType::Strength:
      explorer->increaseStrength(bonus);
      break;
    }

    // Display a message for successful exploration
    messages.push_back("You loot " + labelWithCoords(*treasure) + " for +" +
                       std::to_string(bonus) + ".");
  };

  // Define what happens on the map after treasure exploration
  auto updateMapAfterExploration = [&](const auto &explorer,
                                       const auto &exploredTreasure) {
    map->setCellType(exploredTreasure->position, CellType::EMPTY);
    treasures.erase(exploredTreasure->position);
  };

  // Display a message for starting treasure exploration
  info->addMessage(MessageType::LOOT, &player->position,
                   "You search the treasure...");

  std::vector<std::string> explorationMessages;

  // Explore the treasure
  explore(player, treasure, explorationMessages);

  // Update the map after exploration
  updateMapAfterExploration(player, treasure);

  // Display exploration messages
  info->addMessage(MessageType::LOOT, &player->position, explorationMessages);
}

void Model::queuePlayerMove(const Point &point) { playerMoves.push(point); }

void Model::castPlayerSpell(int spellIndex, const Point &direction) {
  if (!player || !player->isAlive()) {
    return;
  }
  
  if (player->castSpell(spellIndex)) {
    auto spell = player->getSpell(spellIndex);
    if (spell) {
      auto effect = std::make_shared<SpellEffect>(spell, player->position, direction);
      activeSpellEffects.push_back(effect);
      info->addMessage(MessageType::COMBAT, &player->position,
                       "You cast " + spell->getName() + ".");
    }
  } else {
    info->addMessage(MessageType::INFO, &player->position,
                     "Not enough mana.");
  }
}

void Model::updateSpellEffects() {
  // Update all active spell effects
  for (auto &effect : activeSpellEffects) {
    effect->update();
    
    // Check for collisions during traveling state
    if (effect->getState() == EffectState::TRAVELING ||
        effect->getState() == EffectState::IMPACT ||
        effect->getState() == EffectState::EXPANDING) {
      checkSpellCollisions(effect);
    }
  }
  
  // Remove completed effects
  activeSpellEffects.erase(
    std::remove_if(activeSpellEffects.begin(), activeSpellEffects.end(),
                   [](const std::shared_ptr<SpellEffect> &effect) {
                     return effect->isComplete();
                   }),
    activeSpellEffects.end());
}

void Model::checkSpellCollisions(const std::shared_ptr<SpellEffect> &effect) {
  auto frames = effect->getCurrentFrames();
  auto spell = effect->getSpell();
  
  // Handle healing and shield spells (self-cast)
  if (spell->getType() == SpellType::HEAL) {
    player->heal(spell->getDamage());
    info->addMessage(MessageType::INFO, &player->position,
                     "You heal for " + std::to_string(spell->getDamage()) +
                         " HP.");
    return;
  }
  
  if (spell->getType() == SpellType::SHIELD) {
    // Shield effect could be implemented as temporary damage reduction
    info->addMessage(MessageType::INFO, &player->position, "Shield up.");
    return;
  }
  
  // Check for collisions with monsters
  for (const auto &frame : frames) {
    Point pos = frame.position;
    
    // Check if spell hits a wall
    if (isWall(pos)) {
      // Spell stops at wall
      continue;
    }
    
    // Check if spell hits a monster
    for (auto &monster : monsters) {
      if (monster->position == pos && monster->isAlive()) {
        int damage = effect->getDamage();
        monster->takeDamage(damage);
        
        info->addMessage(MessageType::COMBAT, &player->position,
                         spell->getName() + " hits " +
                             labelWithCoords(*monster) + " for " +
                             std::to_string(damage) + ".");
        
        if (!monster->isAlive()) {
          monstersKilled++;
          int expGain = monsterExpMap[monster->cellType];
          int scoreGain = expGain * currentLevel;
          totalScore += scoreGain;
          player->addExperience(expGain);
          info->addMessage(MessageType::COMBAT, &player->position,
                           "You defeated " + labelWithCoords(*monster) +
                               ". +" + std::to_string(expGain) + " EXP");
          map->setCellType(monster->position, CellType::EMPTY);
        }
      }
    }
  }
}

void Model::attemptPlayerMove(const std::shared_ptr<Player> &player,
                              const Point &direction) {
  auto currentPos = player->position;
  auto newPos = currentPos + direction;
  
  // Track player's facing direction for spell casting
  player->setLastDirection(direction);

  if (isWall(newPos)) {
    return;
  } else if (isMonster(newPos)) {
    for (const auto &monster : monsters) {
      if (monster->position == newPos) {
        fight(monster);
        monsters.erase(std::remove(monsters.begin(), monsters.end(), monster),
                       monsters.end());
        break;
      }
    }
    return;
  } else if (isMovableObject(newPos)) {
    // Try to push the object
    if (tryPushObject(newPos, direction)) {
      // If push successful, move player to the object's old position
      updateEntityPosition(player, currentPos, newPos);
    }
    return;
  } else if (isTreasure(newPos)) {
    exploreTreasure(treasures[newPos]);
  }

  else if (isExit(newPos)) {
    restart();
    return;
  }
  updateEntityPosition(player, currentPos, newPos);
}

void Model::attemptMonsterMove(const std::shared_ptr<Monster> &monster,
                               const Point &direction) {
  auto currentPos = monster->position;
  auto newPos = currentPos + direction;

  if (isWall(newPos) || isMonster(newPos) || isExit(newPos)) {
    monster->randomizeVelocity();
    return;
  } else if (isPlayer(newPos)) {
    fight(monster);
    return;
  }

  updateEntityPosition(monster, currentPos, newPos);
}

void Model::updateEntityPosition(const std::shared_ptr<Entity> &entity,
                                 const Point &oldPos, const Point &newPos) {
  auto cellType = map->getCellType(oldPos);
  map->setCellType(oldPos, CellType::EMPTY);
  map->setCellType(newPos, cellType);
  entity->move(newPos);
}

std::unordered_map<std::string, std::string> Model::getPlayerStats() {

  std::unordered_map<std::string, std::string> result;

  // Character stats
  result["Level"] = std::to_string(player->level);
  result["Health"] = std::to_string(player->health);
  result["MaxHealth"] = std::to_string(player->getMaxHealth());
  result["Mana"] = std::to_string(player->getMana());
  result["MaxMana"] = std::to_string(player->getMaxMana());
  result["Experience"] = std::to_string(player->exp);
  result["MaxExp"] = std::to_string(player->expToNextLevel());
  result["Strength"] = std::to_string(player->strength);

  // Game progression stats
  result["DungeonLevel"] = std::to_string(currentLevel);
  result["MonstersKilled"] = std::to_string(monstersKilled);
  result["Score"] = std::to_string(totalScore);
  result["MonstersRemaining"] = std::to_string(monsters.size());

  return result;
}

bool Model::isGameOver() { return !player->isAlive(); }

bool Model::isWall(const Point &point) {
  if (!map->isValidPoint(point)) {
    return true;
  }
  CellType cell = map->getCellType(point);
  return cell == CellType::WALL || cell == CellType::MOUNTAIN || cell == CellType::WATER;
}

bool Model::isPlayer(const Point &point) {
  return map->getCellType(point) == CellType::PLAYER;
}

bool Model::isExit(const Point &point) {
  return map->getCellType(point) == CellType::END;
}

bool Model::isTreasure(const Point &point) {
  return map->getCellType(point) == CellType::TREASURE;
}

bool Model::isMonster(const Point &point) {
  CellType cell = map->getCellType(point);
  return cell == CellType::GOBLIN ||
         cell == CellType::ORC ||
         cell == CellType::TROLL ||
         cell == CellType::DRAGON ||
         cell == CellType::SKELETON;
}

bool Model::isMovableObject(const Point &point) {
  CellType cell = map->getCellType(point);
  return cell == CellType::BOULDER ||
         cell == CellType::CRATE ||
         cell == CellType::BARREL;
}

bool Model::tryPushObject(const Point &objectPos, const Point &direction) {
  // Check if there's actually a movable object at this position
  if (!isMovableObject(objectPos) || movableObjects.find(objectPos) == movableObjects.end()) {
    return false;
  }
  
  auto object = movableObjects[objectPos];
  Point newPos = objectPos + direction;
  
  // Check if the new position is free
  if (!map->isPositionFree(newPos) || isMonster(newPos) || 
      isPlayer(newPos) || isMovableObject(newPos) || isTreasure(newPos)) {
    return false;
  }
  
  // Move the object
  map->setCellType(objectPos, CellType::EMPTY);
  map->setCellType(newPos, object->cellType);
  object->move(newPos);
  
  // Update the map
  movableObjects.erase(objectPos);
  movableObjects[newPos] = object;
  
  info->addMessage(MessageType::INFO, &player->position,
                   "You push " + labelWithCoords(*object) + ".");
  return true;
}

void Model::updateTraps() {
  for (auto &trap : traps) {
    trap->update();
  }
}

void Model::checkTrapCollisions() {
  for (auto &trap : traps) {
    const auto &projectiles = trap->getProjectiles();
    
    for (size_t i = 0; i < projectiles.size(); ++i) {
      const auto &proj = projectiles[i];
      if (!proj.active) continue;
      
      Point pos = proj.position;
      
      // Check if projectile is out of bounds or hits a wall
      if (!map->isValidPoint(pos) || isWall(pos)) {
        trap->deactivateProjectile(i);
        continue;
      }
      
      // Check if projectile hits the player
      if (isPlayer(pos)) {
        player->takeDamage(proj.damage);
        info->addMessage(MessageType::COMBAT, &trap->position,
                         labelWithCoords(*trap) + " hits you for " +
                             std::to_string(proj.damage) + ".");
        
        if (!player->isAlive()) {
          info->addMessage(MessageType::SYSTEM, &player->position,
                           "You were killed by " + labelWithCoords(*trap) +
                               ".");
          map->setCellType(player->position, CellType::EMPTY);
        }
        trap->deactivateProjectile(i);
        continue;
      }
      
      // Check if projectile hits a monster
      for (auto &monster : monsters) {
        if (monster->position == pos && monster->isAlive()) {
          monster->takeDamage(proj.damage);
          info->addMessage(MessageType::COMBAT, &trap->position,
                           labelWithCoords(*trap) + " hits " +
                               labelWithCoords(*monster) + " for " +
                               std::to_string(proj.damage) + ".");
          
          if (!monster->isAlive()) {
            map->setCellType(monster->position, CellType::EMPTY);
          }
          trap->deactivateProjectile(i);
          break;
        }
      }
    }
  }
}
