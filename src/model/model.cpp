#include "model.h"
#include "utils/global_config.h"
#include <chrono>
#include <queue>

std::queue<Point> playerMoves;
std::chrono::steady_clock::time_point lastUpdate =
    std::chrono::steady_clock::now();

Model::Model() : running(false) {}

void Model::restart() {

  if (!player || !player->isAlive()) {
    player = std::make_shared<Player>();
  }
  map = std::make_shared<Map>(100, 100);
  info = std::make_shared<InfoDeque>(20);

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

  if (elapsed.count() < 360) {
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

  while (monster->isAlive() && player->isAlive()) {

    auto damageToPlayer = monster->getAttack();
    auto damageToMonster = player->getAttack();

    player->takeDamage(damageToPlayer);
    monster->takeDamage(damageToMonster);

    std::vector<std::string> fightMessage;

    fightMessage.push_back("Player hit the " + monster->toString() + ": " +
                           std::to_string(damageToMonster) + ".");
    fightMessage.push_back(monster->toString() + " hit the player: " +
                           std::to_string(damageToPlayer) + ".");

    if (!monster->isAlive()) {
      fightMessage.push_back(monster->toString() + " was defeated!");
      map->setCellType(monster->position, CellType::EMPTY);
      map->setCellType(player->position, player->cellType);
    } else if (!player->isAlive()) {
      fightMessage.push_back("Player was defeated!");
      map->setCellType(monster->position, monster->cellType);
      map->setCellType(player->position, CellType::EMPTY);
    }

    info->addMessage(fightMessage);
  }
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
  auto health = player->getHealth();
  auto level = player->level;
  auto exp = player->getExp();

  result["Health"] = std::to_string(health);
  result["Level"] = std::to_string(level);
  result["Experience"] = std::to_string(exp);

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
