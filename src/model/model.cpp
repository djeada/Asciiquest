#include "model.h"

Model::Model() {
  int rows = 10;
  int cols = 10;
  grid.resize(rows, std::vector<CellType>(cols));
  // Fill the entire grid with CellType::EMPTY
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      grid[i][j] = CellType::EMPTY;
    }
  }

  // Top and bottom borders
  for (int j = 0; j < cols; ++j) {
    grid[0][j] = CellType::WALL;
    grid[rows - 1][j] = CellType::WALL;
  }

  // Left and right borders
  for (int i = 0; i < rows; ++i) {
    grid[i][0] = CellType::WALL;
    grid[i][cols - 1] = CellType::WALL;
  }
  grid[1][1] = CellType::PLAYER;
}

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

void Model::movePlayer(Point point) {

  auto wishedNewPos = player.nextMove();
  if (!isWall(wishedNewPos))
    player.move(wishedNewPos);
}

bool Model::isWall(Point point) { return false; }

bool Model::isPlayer(Point point) { return false; }

std::unordered_map<std::string, std::string> Model::getPlayerStats() {
  std::unordered_map<std::string, std::string> result;
  return result;
}

void Model::loadMap() {
  // changes the grid
  // must adjust the dimensions as well
  //     grid.resize(rows, std::vector<CellType>(cols));
}

bool Model::isGameOver() { return player.isAlive(); }

void Model::updateEntityPosition(Point oldPos, Point newPos) {
  auto cell_type = grid[oldPos.x][oldPos.y];
  grid[oldPos.x][oldPos.y] = CellType::EMPTY;
  grid[newPos.x][newPos.y] = cell_type;
}
