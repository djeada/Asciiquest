#include "model.h"

Model::Model() {}

void Model::update() {

  // maybe gather all new positions, then start fight

  for (auto monster : monsters) {

    auto wishedNewPos = monster.nextMove();

    if (isWall(wishedNewPos))
      continue;

    if (isPlayer(wishedNewPos)) {
      if (isGameOver())
        return;
    }

    monster.move(wishedNewPos);
  }

  // cleanup dead enemies
}

void Model::fight(Monster &monster) {

  // player vs enemy fight
  // fills fight info
}

void Model::movePlayer(Direction direction) {

  auto wishedNewPos = player.nextMove();
  if (!isWall(wishedNewPos))
    player.move(wishedNewPos);
}

bool Model::isWall(Point point) { return false; }

bool Model::isPlayer(Point point) { return false; }

std::unordered_map<std::string, std::string> getPlayerStats() {
  std::unordered_map<std::string, std::string> result;
  return result;
}

void Model::loadMap() {
  // changes the grid
  // must adjust the dimensions as well
}

bool Model::isGameOver() { return player.isAlive(); }

void Model::updateEntityPosition(Point oldPos, Point newPos) {
  auto cell_type = grid[oldPos.x][oldPos.y];
  grid[oldPos.x][oldPos.y] = CellType::EMPTY;
  grid[newPos.x][newPos.y] = cell_type;
}
