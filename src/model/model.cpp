#include "model.h"
#include "utils/global_config.h"
#include <chrono>
#include <queue>

const int monsterUpdateSpeed =
    GlobalConfig::getInstance().getConfig<int>("MonsterUpdateSpeed");
Model::Model() : running(false), lastUpdate(std::chrono::steady_clock::now()) {}

void Model::restart() {

  if (!player || !player->isAlive()) {
    player = std::make_shared<Player>();
  }
  map = std::make_shared<Map>(
      GlobalConfig::getInstance().getConfig<int>("MapWidth"),
      GlobalConfig::getInstance().getConfig<int>("MapHeight"));
  info = std::make_shared<InfoDeque>(
      GlobalConfig::getInstance().getConfig<int>("MessageQueueSize"));

  auto addMonsters = [this](const std::string &type, auto monsterMaker) {
    const int monsterCount = GlobalConfig::getInstance().getConfig<int>(type);
    monsters.reserve(monsters.size() + monsterCount);

    for (int i = 0; i < monsterCount; i++) {
      monsters.push_back(
          std::make_shared<decltype(monsterMaker)>(monsterMaker));
    }
  };

  addMonsters("GoblinsCount", Goblin());
  addMonsters("TrollsCount", Troll());
  addMonsters("DragonsCount", Dragon());

  loadMap();
}

void Model::loadMap() {
  map->loadLevel();
  map->setCellType(map->getStart(), CellType::PLAYER);
  player->move(map->getStart());

  for (const auto &monster : monsters) {
    auto pos = map->randomFreePosition();
    monster->position = pos;
    map->setCellType(pos, monster->cellType);
  }
  map->setCellType(map->getEnd(), CellType::END);
  info->addMessage("Welcome on the new level!");
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

  if (elapsed.count() < monsterUpdateSpeed) {
    return;
  }

  for (const auto &monster : monsters) {
    attemptMonsterMove(monster, monster->velocity);
  }

  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                [](const std::shared_ptr<Monster> &monster) {
                                  return !monster->isAlive();
                                }),
                 monsters.end());

  lastUpdate = now;
}

void Model::fight(const std::shared_ptr<Monster> &monster) {

  auto attack = [&](const auto &attacker, const auto &defender,
                    auto &messages) {
    double successRate = (rand() % 100) / 100.0; // random value between 0 and 1
    if (successRate > 0.85) {                    // 15% chance of attack missing
      messages.push_back(attacker->toString() + " misses " +
                         defender->toString() + ".");
      return;
    }

    int damage = attacker->strength * successRate; // add randomness to damage
    if (successRate < 0.1) { // 10% chance of attack being blocked
      damage = 0;
      messages.push_back(defender->toString() + " blocks " +
                         attacker->toString() + "'s attack.");
    } else {
      defender->takeDamage(damage);
      messages.push_back(attacker->toString() + " hits " +
                         defender->toString() + " for " +
                         std::to_string(damage) + " damage.");
    }

    if (!monster->isAlive()) {
      messages.push_back(monster->toString() + " was defeated!");
      player->addExperience(monsterExpMap[monster->cellType]);

    } else if (!player->isAlive()) {
      messages.push_back("Player was defeated!");
    }
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

  info->addMessage("New fight starts!");

  while (player->isAlive() && monster->isAlive()) {
    std::vector<std::string> roundMessages;

    attack(player, monster, roundMessages);
    if (monster->isAlive()) {
      attack(monster, player, roundMessages);
    }

    info->addMessage(roundMessages);
  }

  updateMapAfterFight(monster);
}

void Model::queuePlayerMove(const Point &point) { playerMoves.push(point); }

void Model::attemptPlayerMove(const std::shared_ptr<Player> &player,
                              const Point &direction) {
  auto currentPos = player->position;
  auto newPos = currentPos + direction;

  if (isWall(newPos) || isMonster(newPos)) {
    for (const auto &monster : monsters) {
      if (monster->position == newPos) {
        fight(monster);
        monsters.erase(std::remove(monsters.begin(), monsters.end(), monster),
                       monsters.end());
        break;
      }
    }
    return;
  } else if (isExit(newPos)) {
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

  result["Health"] = std::to_string(player->health);
  result["Level"] = std::to_string(player->level);
  result["Experience"] = std::to_string(player->exp);

  return result;
}

bool Model::isGameOver() { return !player->isAlive(); }

bool Model::isWall(const Point &point) {
  return map->getCellType(point) == CellType::WALL || !map->isValidPoint(point);
}

bool Model::isPlayer(const Point &point) {
  return map->getCellType(point) == CellType::PLAYER;
}

bool Model::isExit(const Point &point) {
  return map->getCellType(point) == CellType::END;
}

bool Model::isMonster(const Point &point) {
  return map->getCellType(point) == CellType::GOBLIN ||
         map->getCellType(point) == CellType::ORC ||
         map->getCellType(point) == CellType::TROLL ||
         map->getCellType(point) == CellType::DRAGON;
}
