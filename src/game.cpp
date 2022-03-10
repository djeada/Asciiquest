#include "game.h"
#include "monster.h"
#include <csignal>
#include <cstdlib>
#include <functional>

Game::Game() {
  int maxWidth = 10; 
  int maxHeight = 10;
  initscr();
  getmaxyx(stdscr, maxHeight, maxWidth);
  map = std::make_unique<Map>(maxWidth, maxHeight - 1);
  player = Player(map->getStart());
}

Game::~Game() {}

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
  auto createMonster =
      [&](int count,
          const std::function<std::unique_ptr<Monster>(Point)> &builder) {
        for (int i = 0; i < count; i++) {
          Point position = map->randomFreePosition();
          monsters.push_back(builder(position));
        }
      };

  createMonster(goblinsCount, [](const Point &position) {
    return std::make_unique<Goblin>(position);
  });
  createMonster(orcsCount, [](const Point &position) {
    return std::make_unique<Orc>(position);
  });
  createMonster(trollsCount, [](const Point &position) {
    return std::make_unique<Troll>(position);
  });
  createMonster(dragonsCount, [](const Point &position) {
    return std::make_unique<Dragon>(position);
  });
}

void Game::updateEntityPosition(Entity &entity, int dx, int dy) {

  auto oldPos = entity.getPosition();
  entity.move(dx, dy);

  if (!map->isPositionFree(entity.getPosition())) {
    entity.setPosition(oldPos);
  }
}

void Game::run() {
  init();
  while (!isGameOver()) {
    render();
    updatePositions();
    handleInput();
    if (isLevelComplete()) {
      loadLevel();
    }
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
    updateEntityPosition(*monster, 1, 1);
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
    raise(SIGQUIT);
  case KEY_UP:
  case 'w':
    updateEntityPosition(player, 0, -1);
    break;
  case KEY_DOWN:
  case 's':
    updateEntityPosition(player, 0, 1);
    break;
  case KEY_LEFT:
  case 'a':
    updateEntityPosition(player, -1, 0);
    break;
  case KEY_RIGHT:
  case 'd':
    updateEntityPosition(player, 1, 0);
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

  if (defender.isAlive()) {
    return;
  }

  // check if player is dead
  if (!player.isAlive()) {
    gameOver();
    return;
  }

  // someone died and it's not the player
  // update player exp
  player.setExp(player.getExp() + 10);

  // remove the dead entity from the map
  /*
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                  [&defender](std::unique_ptr<Monster> monster)
    { return monster->getPosition() == defender.getPosition();
                                  }),
                   monsters.end());
  */
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
  raise(SIGQUIT);
}

auto Game::isGameOver() -> bool { return !player.isAlive(); }

auto Game::isLevelComplete() -> bool {
  return player.getPosition() == map->getEnd();
}
