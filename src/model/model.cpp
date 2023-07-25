#include "model.h"
#include <chrono>
#include <sstream>
#include <thread>

Model::Model() : running(false) {
  map = std::make_shared<Map>(100, 100);

  for (int i = 0; i < 100; ++i) {
    auto goblin = std::make_shared<Goblin>(Point(i, i));
    monsters.push_back(goblin);
  }

  loadMap();
  startUpdateLoop();
}

void Model::startUpdateLoop() {
  running = true;

  std::thread updateThread([this]() {
    while (running) {
      this->update();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  updateThread.detach();
}

void Model::stopUpdateLoop() {
  running = false;
  // if (updateThread.joinable()) {
  //   updateThread.join();
  //}
}

void Model::update() {

  for (auto &monster : monsters) {
    moveEntity(monster->velocity, *monster);
    if (isPlayer(monster->getPosition())) {
      fight(*monster);
    }
  }

  std::lock_guard<std::mutex> lock(fightMutex);
  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                [](const std::shared_ptr<Monster> &monster) {
                                  return !monster->isAlive();
                                }),
                 monsters.end());
}

void Model::fight(Monster &monster) {

  info.clear();
  running = false;
  if (!monster.isAlive()) {
    return;
  }
  std::lock_guard<std::mutex> lock(fightMutex);
  while (monster.isAlive() && player.isAlive()) {
    int damageToPlayer = monster.getAttack();
    int damageToMonster = player.getAttack();

    player.setHealth(player.getHealth() - damageToPlayer);
    monster.setHealth(monster.getHealth() - damageToMonster);

    std::stringstream ss;
    ss << "Player hit the monster: " << damageToMonster << ".";
    info.push_back(ss.str());

    ss.str(std::string());
    ss << "Monster hit the player: " << damageToPlayer << ".";
    info.push_back(ss.str());
  }

  running = true;

  if (!monster.isAlive()) {
    info.push_back("Monster was defeated!");
    map->setCellType(monster.getPosition(), CellType::EMPTY);
    map->setCellType(player.getPosition(), CellType::PLAYER);
  } else {
    info.push_back("Player was defeated!");
  }
}

void Model::movePlayer(Point point) {
  // Get the new position first to check if there's a monster there
  auto newPos = player.getPosition() + point;

  // If there is a monster at the new position, initiate a fight
  if (isMonster(newPos)) {
    for (auto &monster : monsters) {
      if (monster->getPosition() == newPos) {
        fight(*monster);
        break;
      }
    }
  } else {
    // If there is no monster, move the player normally
    moveEntity(point, player);
  }
}

void Model::moveEntity(Point point, Entity &entity) {
  auto oldPos = entity.getPosition();
  entity.moveBy(point);
  auto newPos = entity.getPosition();

  if (isWall(newPos)) {
    entity.move(oldPos);
    return;
  }

  updateEntityPosition(oldPos, entity.getPosition());
}

bool Model::isWall(Point point) {
  return map->getCellType(point) == CellType::WALL || !map->isValidPoint(point);
}

bool Model::isPlayer(Point point) {
  return map->getCellType(point) == CellType::PLAYER;
}

bool Model::isMonster(Point point) {
  return map->getCellType(point) == CellType::GOBLIN;
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

void Model::loadMap() {
  map->loadLevel();

  map->setCellType(map->getStart(), CellType::PLAYER);
  player.setPosition(map->getStart());

  for (auto &monster : monsters) {
    auto pos = map->randomFreePosition();
    monster->setPosition(pos);
    map->setCellType(pos, CellType::GOBLIN);
  }
}

bool Model::isGameOver() { return !player.isAlive(); }

void Model::updateEntityPosition(Point oldPos, Point newPos) {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto cellType = map->getCellType(oldPos);
  map->setCellType(oldPos, CellType::EMPTY);
  map->setCellType(newPos, cellType);
}
