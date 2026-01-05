#include "model.h"
#include "utils/global_config.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <queue>
#include <random>
#include <unordered_set>

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

  auto addMonstersWithMapAndPlayer = [this](const std::string &type, 
                                             std::function<std::shared_ptr<Monster>()> monsterMaker) {
    int monsterCount = GlobalConfig::getInstance().getConfig<int>(type);
    // Scale monster count with level (up to 50% more monsters at higher levels)
    monsterCount = monsterCount + (monsterCount * (currentLevel - 1) / 10);
    monsters.reserve(monsters.size() + monsterCount);

    for (int i = 0; i < monsterCount; i++) {
      auto monster = monsterMaker();
      // Scale monster health and damage with difficulty
      int diffMult = getDifficultyMultiplier();
      monster->health = (monster->health * diffMult) / 100;
      monster->strength = (monster->strength * diffMult) / 100;
      monsters.push_back(monster);
    }
  };

  addMonstersWithMapAndPlayer("GoblinsCount", [this]() { 
    return std::make_shared<Goblin>(map, player); 
  });
  addMonstersWithMapAndPlayer("TrollsCount", [this]() { 
    return std::make_shared<Troll>(map, player); 
  });
  addMonstersWithMapAndPlayer("SkeletonsCount", [this]() { 
    return std::make_shared<Skeleton>(map, player); 
  });
  addMonstersWithMapAndPlayer("OrcsCount", [this]() { 
    return std::make_shared<Orc>(map, player); 
  });

  // Adding Dragons separately as they don't need map and player
  int dragonsCount = GlobalConfig::getInstance().getConfig<int>("DragonsCount");
  dragonsCount = dragonsCount + (dragonsCount * (currentLevel - 1) / 10);
  monsters.reserve(monsters.size() + dragonsCount);
  for (int i = 0; i < dragonsCount; i++) {
    auto dragon = std::make_shared<Dragon>();
    int diffMult = getDifficultyMultiplier();
    dragon->health = (dragon->health * diffMult) / 100;
    dragon->strength = (dragon->strength * diffMult) / 100;
    monsters.push_back(dragon);
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
  player->underlyingCell = map->getCellType(map->getStart());
  map->setCellType(map->getStart(), CellType::PLAYER);
  player->move(map->getStart());

  for (const auto &monster : monsters) {
    auto position = map->randomFreePosition();
    monster->position = position;
    monster->underlyingCell = map->getCellType(position);
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

  // Spawn potions (player-only pickups)
  int potionCount =
      GlobalConfig::getInstance().getConfig<int>("PotionCount");
  potions.clear();
  std::random_device rd;
  std::mt19937 potionRng(rd());
  std::uniform_int_distribution<int> potionTypeDist(0, 99);
  int manaChance =
      GlobalConfig::getInstance().getConfig<int>("PotionManaChance");

  for (int i = 0; i < potionCount; ++i) {
    auto position = map->randomFreePosition();
    PotionType type = potionTypeDist(potionRng) < manaChance
                          ? PotionType::MANA
                          : PotionType::HEALTH;
    potions.emplace(position, type);
    map->setCellType(position, CellType::POTION);
  }

  map->setCellType(map->getEnd(), CellType::END);

  // Spawn movable objects strategically to block pockets
  movableObjects.clear();
  placeBlockingObjects();

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
  int trapCount = 1 + (currentLevel / 3); // Fewer traps at higher levels
  
  // Add blade traps
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    auto trap = std::make_shared<BladeTrap>(position);
    traps.push_back(trap);
  }
  
  // Add spike traps
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    auto trap = std::make_shared<SpikeTrap>(position);
    traps.push_back(trap);
  }
  
  // Add arrow traps (pointing in different directions)
  for (int i = 0; i < trapCount; ++i) {
    auto position = map->randomFreePosition();
    Point direction = (i % 2 == 0) ? Direction::DOWN : Direction::RIGHT;
    auto trap = std::make_shared<ArrowTrap>(position, direction);
    traps.push_back(trap);
  }
}

std::vector<Point> Model::findPath(const Point &start, const Point &end) const {
  // BFS pathfinding that respects current map state
  if (!map->isValidPoint(start) || !map->isValidPoint(end)) {
    return {};
  }
  
  auto isWalkableCell = [this](const Point &p) {
    if (!map->isValidPoint(p)) return false;
    CellType cell = map->getCellType(p);
    return cell == CellType::FLOOR || cell == CellType::DOOR ||
           cell == CellType::GRASS || cell == CellType::TREE ||
           cell == CellType::DESERT || cell == CellType::END ||
           cell == CellType::PLAYER || cell == CellType::START ||
           cell == CellType::GOBLIN || cell == CellType::ORC ||
           cell == CellType::TROLL || cell == CellType::DRAGON ||
           cell == CellType::SKELETON || cell == CellType::TREASURE ||
           cell == CellType::POTION;
  };
  
  if (!isWalkableCell(start)) {
    return {};
  }
  
  std::queue<Point> frontier;
  std::vector<std::vector<bool>> visited(
      map->getHeight(), std::vector<bool>(map->getWidth(), false));
  std::vector<std::vector<Point>> prev(
      map->getHeight(), std::vector<Point>(map->getWidth(), {-1, -1}));
  
  frontier.push(start);
  visited[start.y][start.x] = true;
  
  const std::array<Point, 4> directions = {
      Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
  
  while (!frontier.empty()) {
    Point current = frontier.front();
    frontier.pop();
    
    if (current == end) {
      // Reconstruct path
      std::vector<Point> path;
      Point step = end;
      while (step != start) {
        path.push_back(step);
        step = prev[step.y][step.x];
        if (!map->isValidPoint(step)) break;
      }
      path.push_back(start);
      std::reverse(path.begin(), path.end());
      return path;
    }
    
    for (const auto &dir : directions) {
      Point next{current.x + dir.x, current.y + dir.y};
      if (!map->isValidPoint(next) || visited[next.y][next.x]) continue;
      if (!isWalkableCell(next) && next != end) continue;
      
      visited[next.y][next.x] = true;
      prev[next.y][next.x] = current;
      frontier.push(next);
    }
  }
  
  return {};
}

std::vector<Point> Model::findPathIgnoringMovables(const Point &start, const Point &end) const {
  // BFS pathfinding that treats movable objects as walkable (can be pushed)
  if (!map->isValidPoint(start) || !map->isValidPoint(end)) {
    return {};
  }
  
  auto isWalkableOrPushable = [this](const Point &p) {
    if (!map->isValidPoint(p)) return false;
    CellType cell = map->getCellType(p);
    return cell == CellType::FLOOR || cell == CellType::DOOR ||
           cell == CellType::GRASS || cell == CellType::TREE ||
           cell == CellType::DESERT || cell == CellType::END ||
           cell == CellType::PLAYER || cell == CellType::START ||
           cell == CellType::CRATE || cell == CellType::BARREL ||
           cell == CellType::BOULDER ||
           cell == CellType::GOBLIN || cell == CellType::ORC ||
           cell == CellType::TROLL || cell == CellType::DRAGON ||
           cell == CellType::SKELETON || cell == CellType::TREASURE ||
           cell == CellType::POTION;
  };
  
  std::queue<Point> frontier;
  std::vector<std::vector<bool>> visited(
      map->getHeight(), std::vector<bool>(map->getWidth(), false));
  std::vector<std::vector<Point>> prev(
      map->getHeight(), std::vector<Point>(map->getWidth(), {-1, -1}));
  
  frontier.push(start);
  visited[start.y][start.x] = true;
  
  const std::array<Point, 4> directions = {
      Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
  
  while (!frontier.empty()) {
    Point current = frontier.front();
    frontier.pop();
    
    if (current == end) {
      std::vector<Point> path;
      Point step = end;
      while (step != start) {
        path.push_back(step);
        step = prev[step.y][step.x];
        if (!map->isValidPoint(step)) break;
      }
      path.push_back(start);
      std::reverse(path.begin(), path.end());
      return path;
    }
    
    for (const auto &dir : directions) {
      Point next{current.x + dir.x, current.y + dir.y};
      if (!map->isValidPoint(next) || visited[next.y][next.x]) continue;
      if (!isWalkableOrPushable(next) && next != end) continue;
      
      visited[next.y][next.x] = true;
      prev[next.y][next.x] = current;
      frontier.push(next);
    }
  }
  
  return {};
}

void Model::placeBlockingObjects() {
  // Find the path from start to exit
  Point startPos = map->getStart();
  Point endPos = map->getEnd();
  
  std::vector<Point> mainPath = findPath(startPos, endPos);
  if (mainPath.empty()) {
    // No path exists, just place random objects
    int objectsPerType = 2 + (currentLevel / 2);
    for (int i = 0; i < objectsPerType; ++i) {
      auto position = map->randomFreePosition();
      auto crate = std::make_shared<Crate>(position);
      crate->underlyingCell = map->getCellType(position);
      movableObjects.emplace(position, crate);
      map->setCellType(position, CellType::CRATE);
    }
    return;
  }
  
  // Create a set for O(1) path lookup
  std::unordered_set<Point> mainPathSet(mainPath.begin(), mainPath.end());
  
  std::random_device rd;
  std::mt19937 rng(rd());
  
  // Find chokepoints on the main path (narrow passages where we can block)
  std::vector<Point> chokepoints;
  const std::array<Point, 4> directions = {
      Point{1, 0}, Point{-1, 0}, Point{0, 1}, Point{0, -1}};
  
  auto isBlockable = [this](const Point &p) {
    if (!map->isValidPoint(p)) return false;
    CellType cell = map->getCellType(p);
    return cell == CellType::FLOOR || cell == CellType::GRASS ||
           cell == CellType::DOOR;
  };
  
  auto isBlocking = [this](const Point &p) {
    if (!map->isValidPoint(p)) return true;
    CellType cell = map->getCellType(p);
    return cell == CellType::WALL || cell == CellType::WATER ||
           cell == CellType::MOUNTAIN;
  };
  
  // Find narrow passages on the path (chokepoints)
  for (size_t i = 1; i + 1 < mainPath.size(); ++i) {
    const Point &pos = mainPath[i];
    
    // Skip positions too close to start or end
    int distToStart = std::abs(pos.x - startPos.x) + std::abs(pos.y - startPos.y);
    int distToEnd = std::abs(pos.x - endPos.x) + std::abs(pos.y - endPos.y);
    if (distToStart < 3 || distToEnd < 3) continue;
    
    // Check if this is a narrow passage (walls/water on two opposite sides)
    bool horizontalNarrow = isBlocking(Point{pos.x - 1, pos.y}) && 
                            isBlocking(Point{pos.x + 1, pos.y});
    bool verticalNarrow = isBlocking(Point{pos.x, pos.y - 1}) && 
                          isBlocking(Point{pos.x, pos.y + 1});
    
    if ((horizontalNarrow || verticalNarrow) && isBlockable(pos)) {
      chokepoints.push_back(pos);
    }
  }
  
  // Find pocket entrances (areas that can be blocked off)
  std::vector<std::pair<Point, Point>> pocketEntrances; // (entrance, direction toward pocket)
  
  for (int y = 1; y < static_cast<int>(map->getHeight()) - 1; ++y) {
    for (int x = 1; x < static_cast<int>(map->getWidth()) - 1; ++x) {
      Point pos{x, y};
      if (!isBlockable(pos)) continue;
      
      // Check each direction for a potential pocket
      for (const auto &dir : directions) {
        Point behind{pos.x - dir.x, pos.y - dir.y};
        Point ahead{pos.x + dir.x, pos.y + dir.y};
        
        if (!map->isValidPoint(behind) || !map->isValidPoint(ahead)) continue;
        
        // Check if behind is walkable (the pocket)
        CellType behindCell = map->getCellType(behind);
        bool behindWalkable = behindCell == CellType::FLOOR || 
                              behindCell == CellType::GRASS ||
                              behindCell == CellType::DOOR;
        
        // Check if ahead is walkable (where player approaches from)
        CellType aheadCell = map->getCellType(ahead);
        bool aheadWalkable = aheadCell == CellType::FLOOR || 
                             aheadCell == CellType::GRASS ||
                             aheadCell == CellType::DOOR ||
                             aheadCell == CellType::PLAYER;
        
        if (behindWalkable && aheadWalkable) {
          // Check if the sides are blocked (forming a pocket entrance)
          Point side1{pos.x + dir.y, pos.y + dir.x};
          Point side2{pos.x - dir.y, pos.y - dir.x};
          
          if (isBlocking(side1) && isBlocking(side2)) {
            pocketEntrances.push_back({pos, dir});
          }
        }
      }
    }
  }
  
  // Place blocking objects
  int blockedPockets = 0;
  int pathBlockedPockets = 0;
  const int minBlockedPockets = 2;
  const int maxPocketEntranceBlocks = 3; // minBlockedPockets + 1 extra for variety
  const int minPathBlockedPockets = 1;
  
  std::shuffle(chokepoints.begin(), chokepoints.end(), rng);
  std::shuffle(pocketEntrances.begin(), pocketEntrances.end(), rng);
  
  // First, place a blocking object on the path (at a chokepoint)
  for (const auto &pos : chokepoints) {
    if (pathBlockedPockets >= minPathBlockedPockets) break;
    
    // Place a crate or barrel at this chokepoint
    auto crate = std::make_shared<Crate>(pos);
    crate->underlyingCell = map->getCellType(pos);
    movableObjects.emplace(pos, crate);
    map->setCellType(pos, CellType::CRATE);
    
    // Verify path still exists after pushing
    std::vector<Point> pathAfter = findPathIgnoringMovables(startPos, endPos);
    if (!pathAfter.empty()) {
      blockedPockets++;
      pathBlockedPockets++;
    } else {
      // Remove the object if it blocks completely
      movableObjects.erase(pos);
      map->setCellType(pos, crate->underlyingCell);
    }
  }
  
  // Then place objects at pocket entrances
  for (const auto &[pos, dir] : pocketEntrances) {
    if (blockedPockets >= maxPocketEntranceBlocks) break;
    
    // Check if position is already occupied
    if (movableObjects.find(pos) != movableObjects.end()) continue;
    
    // Alternate between crates and barrels
    std::shared_ptr<MovableObject> obj;
    if (blockedPockets % 2 == 0) {
      obj = std::make_shared<Barrel>(pos);
    } else {
      obj = std::make_shared<Crate>(pos);
    }
    
    obj->underlyingCell = map->getCellType(pos);
    movableObjects.emplace(pos, obj);
    map->setCellType(pos, obj->cellType);
    
    // Verify path still exists after pushing
    std::vector<Point> pathAfter = findPathIgnoringMovables(startPos, endPos);
    if (!pathAfter.empty()) {
      blockedPockets++;
      
      // Check if this pocket is on a path to the exit
      bool onPath = mainPathSet.count(pos) > 0;
      if (onPath && pathBlockedPockets < minPathBlockedPockets) {
        pathBlockedPockets++;
      }
    } else {
      // Remove the object if it blocks completely
      movableObjects.erase(pos);
      map->setCellType(pos, obj->underlyingCell);
    }
  }
  
  // If we still need more blocked pockets, place them at random locations
  // that create meaningful blocking
  int attempts = 0;
  while (blockedPockets < minBlockedPockets && attempts < 50) {
    attempts++;
    
    Point pos = map->randomFreePosition();
    if (movableObjects.find(pos) != movableObjects.end()) continue;
    
    // Skip positions too close to start or end
    int distToStart = std::abs(pos.x - startPos.x) + std::abs(pos.y - startPos.y);
    int distToEnd = std::abs(pos.x - endPos.x) + std::abs(pos.y - endPos.y);
    if (distToStart < 3 || distToEnd < 3) continue;
    
    auto crate = std::make_shared<Crate>(pos);
    crate->underlyingCell = map->getCellType(pos);
    movableObjects.emplace(pos, crate);
    map->setCellType(pos, CellType::CRATE);
    
    // Verify path still exists
    std::vector<Point> pathAfter = findPathIgnoringMovables(startPos, endPos);
    if (!pathAfter.empty()) {
      blockedPockets++;
    } else {
      movableObjects.erase(pos);
      map->setCellType(pos, crate->underlyingCell);
    }
  }
  
  // Add a few extra random movable objects for variety
  int extraObjects = 1 + (currentLevel / 2);
  int extraAttempts = 0;
  for (int i = 0; i < extraObjects && extraAttempts < 50; ++i) {
    extraAttempts++;
    
    Point pos = map->randomFreePosition();
    if (movableObjects.find(pos) != movableObjects.end()) {
      // Position occupied, try again without counting this iteration
      --i;
      continue;
    }
    
    auto boulder = std::make_shared<Boulder>(pos);
    boulder->underlyingCell = map->getCellType(pos);
    movableObjects.emplace(pos, boulder);
    map->setCellType(pos, CellType::BOULDER);
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

  auto attack = [&](const auto &attacker, const auto &defender) {
    double successRate = (rand() % 100) / 100.0; // random value between 0 and 1
    bool attackerIsPlayer =
        (static_cast<const void *>(attacker.get()) ==
         static_cast<const void *>(player.get()));
    bool defenderIsPlayer =
        (static_cast<const void *>(defender.get()) ==
         static_cast<const void *>(player.get()));

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
    
    // Enhanced combat feedback with damage and remaining HP
    std::string hpInfo = defender->isAlive() ? " (" + std::to_string(defender->health) + " HP left)" : " (defeated!)";
    info->addMessage(MessageType::COMBAT, &attacker->position,
                     attackerLabel + " hit " + defenderLabel + " for " +
                         std::to_string(damage) + hpInfo);
  };

  auto updateMapAfterFight = [&](const auto &defeatedMonster) {
    if (!defeatedMonster->isAlive()) {
      map->setCellType(defeatedMonster->position,
                       defeatedMonster->underlyingCell);
    }

    if (!player->isAlive()) {
      map->setCellType(player->position, player->underlyingCell);
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
    map->setCellType(exploredTreasure->position, CellType::FLOOR);
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
          map->setCellType(monster->position, monster->underlyingCell);
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
  }

  auto trapIt = std::find_if(
      traps.begin(), traps.end(),
      [&](const std::shared_ptr<Trap> &trap) { return trap->position == newPos; });
  if (trapIt != traps.end()) {
    auto trap = *trapIt;
    int damage = trap->getDamage();
    player->takeDamage(damage);
    info->addMessage(MessageType::COMBAT, &player->position,
                     "You trigger " + trap->toString() + " (-" +
                         std::to_string(damage) + " HP).");

    traps.erase(trapIt);
    if (!player->isAlive()) {
      info->addMessage(MessageType::SYSTEM, &player->position,
                       "You were killed by " + trap->toString() + ".");
      map->setCellType(player->position, player->underlyingCell);
      return;
    }
  }

  if (isMonster(newPos)) {
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
  } else if (isPotion(newPos)) {
    auto potionType = potions[newPos];
    int healAmount = GlobalConfig::getInstance().getConfig<int>("PotionHeal");
    int manaAmount = GlobalConfig::getInstance().getConfig<int>("PotionMana");

    if (potionType == PotionType::HEALTH) {
      player->heal(healAmount);
      info->addMessage(MessageType::LOOT, &player->position,
                       "You drink a potion and heal +" +
                           std::to_string(healAmount) + " HP.");
    } else {
      player->restoreMana(manaAmount);
      info->addMessage(MessageType::LOOT, &player->position,
                       "You drink a potion and restore +" +
                           std::to_string(manaAmount) + " MP.");
    }

    potions.erase(newPos);
    map->setCellType(newPos, CellType::FLOOR);
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

  CellType targetCell = map->getCellType(newPos);
  bool isPlayerOnlyItem =
      targetCell == CellType::TREASURE || targetCell == CellType::POTION;

  if (isWall(newPos) || isMonster(newPos) || isExit(newPos) ||
      isPlayerOnlyItem) {
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
  auto cellType = entity->cellType;
  map->setCellType(oldPos, entity->underlyingCell);
  entity->underlyingCell = map->getCellType(newPos);
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

bool Model::isPotion(const Point &point) {
  return map->getCellType(point) == CellType::POTION;
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
  
  updateEntityPosition(object, objectPos, newPos);
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
          map->setCellType(player->position, player->underlyingCell);
        }
        trap->deactivateProjectile(i);
        continue;
      }
      
      // Traps do not affect monsters.
    }
  }
}
