#include "model.h"

Model::Model() {
  map = std::make_shared<Map>(100, 100);
  loadMap();
}

void Model::update() {
  for (auto &monster : monsters) {
    auto wishedNewPos = monster.nextMove();

    if (isWall(wishedNewPos) || isPlayer(wishedNewPos))
      continue;

    monster.move(wishedNewPos);
  }
}

void Model::fight(Monster &monster) {
  // auto fightOutcome = player.fight(monster);
  // fightInfo.push_back(fightOutcome);
}

void Model::movePlayer(Point point) {

  auto oldPos = player.getPosition();
  player.moveBy(point);
  auto newPos = player.getPosition();

  if (isWall(newPos)) {
    player.move(oldPos);
    return;
  }
  updateEntityPosition(oldPos, player.getPosition());
}

bool Model::isWall(Point point) {
  // Check if point is within the map boundaries
  if (point.x < 0 || point.x >= map->grid[0].size() || point.y < 0 ||
      point.y >= map->grid.size()) {
    // Treat out-of-bounds as a wall
    return true;
  }

  // Return true if cell type is a wall, false otherwise
  return map->getChar(point) == CellType::WALL;
}

bool Model::isPlayer(Point point) {
  return map->getChar(point) == CellType::PLAYER;
}

std::unordered_map<std::string, std::string> Model::getPlayerStats() {

  std::unordered_map<std::string, std::string> result;

  // Get the player's health, level and experience values
  auto health = player.getHealth();
  auto level = player.getLevel();
  auto exp = player.getExp();

  // Convert the integer values to string
  result["Health"] = std::to_string(health);
  result["Level"] = std::to_string(level);
  result["Experience"] = std::to_string(exp);

  return result;
}

void Model::loadMap() {
  map->loadLevel();
  map->setChar(map->getStart(), CellType::PLAYER);
  player.setPosition(map->getStart());
}

bool Model::isGameOver() { return !player.isAlive(); }

void Model::updateEntityPosition(Point oldPos, Point newPos) {
  // Assumes that there is a method in Map to set character at given point
  map->setChar(oldPos, CellType::EMPTY);
  map->setChar(newPos, CellType::PLAYER);
}
