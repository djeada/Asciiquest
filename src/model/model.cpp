#include "model.h"
#include <chrono>
#include <queue>

std::queue<Point> playerMoves;
std::chrono::steady_clock::time_point lastUpdate =
    std::chrono::steady_clock::now();

Model::Model() : running(false) {
  map = std::make_shared<Map>(100, 100);
  info = std::make_shared<InfoDeque>(20);

  for (int i = 0; i < 100; ++i) {
    monsters.push_back(std::make_shared<Goblin>(Point(i, i)));
  }

  loadMap();
}

void Model::loadMap() {
  map->loadLevel();
  map->setCellType(map->getStart(), CellType::PLAYER);
  player.setPosition(map->getStart());

  for (const auto &monster : monsters) {
    auto pos = map->randomFreePosition();
    monster->setPosition(pos);
    map->setCellType(pos, CellType::GOBLIN);
  }
}

void Model::update() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

  while (!playerMoves.empty()) {
    auto offset = playerMoves.front();
    playerMoves.pop();
    attemptEntityMove(player, offset);
  }

  if (elapsed.count() < 360) {
    return;
  }

  for (const auto &monster : monsters) {
    attemptMonsterMove(*monster, monster->velocity);
  }

  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                [](const std::shared_ptr<Monster> &monster) {
                                  return !monster->isAlive();
                                }),
                 monsters.end());
  lastUpdate = now;
}

void Model::fight(Monster &monster) {

  if (!monster.isAlive()) {
    return;
  }

  auto damageToPlayer = monster.getAttack();
  auto damageToMonster = player.getAttack();

  player.setHealth(player.getHealth() - damageToPlayer);
  monster.setHealth(monster.getHealth() - damageToMonster);

  std::vector<std::string> fightMessage;

  fightMessage.push_back(
      "Player hit the monster: " + std::to_string(damageToMonster) + ".");
  fightMessage.push_back(
      "Monster hit the player: " + std::to_string(damageToPlayer) + ".");

  if (!monster.isAlive()) {
    fightMessage.push_back("Monster was defeated!");
    map->setCellType(monster.getPosition(), CellType::EMPTY);
  } else if (!player.isAlive()) {
    fightMessage.push_back("Player was defeated!");
  }

  info->addMessage(fightMessage);
}

void Model::queuePlayerMove(const Point &point) { playerMoves.push(point); }

void Model::attemptPlayerMove(Player &player, const Point &direction) {
  auto currentPos = player.getPosition();
  auto newPos = currentPos + direction;

  if (isWall(newPos) || isMonster(newPos)) {
    for (const auto &monster : monsters) {
      if (monster->getPosition() == newPos) {
        fight(*monster);
        monsters.erase(std::remove(monsters.begin(), monsters.end(), monster),
                       monsters.end());
        break;
      }
    }
    return;
  }
  updateEntityPosition(player, currentPos, newPos);
}

void Model::attemptMonsterMove(Monster &monster, const Point &direction) {
  auto currentPos = monster.getPosition();
  auto newPos = currentPos + direction;

  if (isWall(newPos) || isMonster(newPos)) {
    monster.randomizeVelocity();
    return;
  } else if (isPlayer(newPos)) {
    fight(monster);
    map->setCellType(newPos, CellType::PLAYER);
    return;
  }

  updateEntityPosition(monster, currentPos, newPos);
}

void Model::attemptEntityMove(Entity &entity, const Point &direction) {
  if (Player *player = dynamic_cast<Player *>(&entity)) {
    attemptPlayerMove(*player, direction);
  } else if (Monster *monster = dynamic_cast<Monster *>(&entity)) {
    attemptMonsterMove(*monster, direction);
  }
}

void Model::updateEntityPosition(Entity &entity, const Point &oldPos,
                                 const Point &newPos) {
  auto cellType = map->getCellType(oldPos);
  map->setCellType(oldPos, CellType::EMPTY);
  map->setCellType(newPos, cellType);
  entity.move(newPos);
}

std::unordered_map<std::string, std::string> Model::getPlayerStats() {

  std::unordered_map<std::string, std::string> result;
  auto health = player.getHealth();
  auto level = player.getLevel();
  auto exp = player.getExp();

  result["Health"] = std::to_string(health);
  result["Level"] = std::to_string(level);
  result["Experience"] = std::to_string(exp);

  return result;
}

bool Model::isGameOver() { return !player.isAlive(); }

bool Model::isWall(const Point &point) {
  return map->getCellType(point) == CellType::WALL || !map->isValidPoint(point);
}

bool Model::isPlayer(const Point &point) {
  return map->getCellType(point) == CellType::PLAYER;
}

bool Model::isMonster(const Point &point) {
  return map->getCellType(point) == CellType::GOBLIN;
}
