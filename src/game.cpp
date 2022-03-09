#include "game.h"
#include "monster.h"
#include <cstdlib>
#include <functional>

Game::Game() {
  this->map = new Map();
  player = Player(Point(0, 0), PLAYER_HEALTH, PLAYER_ATTACK);
  loadLevel();
}

Game::~Game() { delete this->map; }

void Game::initalizeMonsters(int count) {

  monsters.clear();

  // 1/2 goblins
  int goblinsCount = count / 2;
  // 1/3 orcs
  int orcsCount = count / 3;
  // 1/6 trolls
  int trollsCount = count / 6;
  // rest dragons
  int dragonsCount = count - goblinsCount - orcsCount - trollsCount;

  // lambda function that creates a Monster object
  // second parameter is function pointer
  auto createMonster = [&](int count, std::function<Monster *(Point)> builder) {
    for (int i = 0; i < count; i++) {
      Point position = map->randomFreePosition();
      monsters.push_back(builder(position));
    }
  };

  createMonster(goblinsCount,
                [](Point position) { return new Goblin(position); });
  createMonster(orcsCount, [](Point position) { return new Orc(position); });
  createMonster(trollsCount,
                [](Point position) { return new Troll(position); });
  createMonster(dragonsCount,
                [](Point position) { return new Dragon(position); });
}

void Game::run() {
  init();
  while (!isGameOver()) {
    updatePositions();
    handleInput();
    if (isLevelComplete()) {
      loadLevel();
    }
    render();
  }
}

void Game::init() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  loadLevel();
}

void Game::updatePositions() {
  for (auto &monster : monsters) {
    monster->move();
    // check if player is in the same position as monster
    if (player.getPosition() == monster->getPosition()) {
      fight(*monster, player);
    }
  }
}

void Game::render() {
  clear();
  map->draw(player);
  player.draw();
  for (auto &monster : monsters) {
    monster->draw();
  }
  refresh();
}

void Game::handleInput() {
  int ch = getch();
  switch (ch) {
  case 'q':
    endwin();
    std::system("clear");

    exit(0);
  case KEY_UP:
  case 'W':
    player.move(0, -1);
    break;
  case KEY_DOWN:
  case 'S':
    player.move(0, 1);
    break;
  case KEY_LEFT:
  case 'A':
    player.move(-1, 0);
    break;
  case KEY_RIGHT:
  case 'D':
    player.move(1, 0);
    break;
  case ' ':
    // check if any monster is in the same position as player
    for (auto &monster : monsters) {
      if (player.getPosition() == monster->getPosition()) {
        fight(player, *monster);
        break;
      }
    }
    break;
  default:
    break;
  }
}

void Game::fight(Entity &attacker, Entity &defender) {
  int damage = attacker.getAttack();
  defender.takeDamage(damage);

  if (defender.isAlive())
    return;

  // check if player is dead
  if (!player.isAlive()) {
    gameOver();
    return;
  }

  // someone died and it's not the player
  // update player exp
  player.setExp(player.getExp() + 10);

  // remove the dead entity from the map
  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                [&defender](Monster *monster) {
                                  return monster->getPosition() ==
                                         defender.getPosition();
                                }),
                 monsters.end());
}

void Game::loadLevel() {
  map->loadLevel();
  player.setPosition(map->getStart());
  initalizeMonsters(MONSTERS_COUNT + pow(2, level - 1));
}

void Game::gameOver() {
  clear();
  map->clear();
  mvprintw(LINES / 2, COLS / 2, "Game Over");
  refresh();
  getch();
  exit(0);
}

bool Game::isGameOver() { return !player.isAlive(); }

bool Game::isLevelComplete() { return player.getPosition() == map->getStart(); }
