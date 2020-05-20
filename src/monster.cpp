#include "monster.h"
#include "utils.h"

Monster::Monster(int _x, int _y, char t) {
  x = _x;
  y = _y;

  tile = t;

  switch (tile) {
  case GOBLIN:
    randomizeVelocity();
    attack = 1;
    health = 1;
    break;

  case ORC:
    randomizeVelocity();
    attack = 3;
    health = 8;
    break;

  case TROLL:
    if (headOrTail())
      vx = 1;
    else
      vx = -1;
    vy = 0;
    attack = 20;
    health = 30;
    break;

  case DRAGON:
    vx = 0;
    vy = 0;
    attack = 100;
    health = 1000;
    break;

  default:
    attack = 0;
    health = 0;
    break;
  }
}

void Monster::draw() {

  switch (tile) {
  case GOBLIN:
    attron(COLOR_PAIR(GOBLIN_COLOR));
    mvprintw(y, x, GOBLIN_TILE);
    attroff(COLOR_PAIR(GOBLIN_COLOR));
    break;

  case ORC:
    attron(COLOR_PAIR(ORC_COLOR));
    mvprintw(y, x, ORC_TILE);
    attroff(COLOR_PAIR(ORC_COLOR));
    break;

  case TROLL:
    attron(COLOR_PAIR(TROLL_COLOR));
    mvprintw(y, x, TROLL_TILE);
    attroff(COLOR_PAIR(TROLL_COLOR));
    break;

  case DRAGON:
    attron(COLOR_PAIR(DRAGON_COLOR));
    mvprintw(y, x, DRAGON_TILE);
    attroff(COLOR_PAIR(DRAGON_COLOR));
    break;

  default:
    break;
  }
}

void Monster::moveMonster(Map myMap, int playerX, int playerY) {
  if (checkNoColisions(myMap, playerX, playerY)) {
    switch (tile) {
    case GOBLIN:
      x += vx;
      y += vy;
      randomizeVelocity();
      break;

    case TROLL:
      x += vx;
      y += vy;
      break;

    case ORC: {
      std::vector<Position> nextPositions =
          findTheRoute(playerX, playerY, x, y, myMap);
      x = nextPositions[nextPositions.size() - 1].x;
      y = nextPositions[nextPositions.size() - 1].y;
      break;
    }

    default:
      break;
    }
  } else
    randomizeVelocity();
}

bool Monster::checkNoColisions(Map myMap, int playerX, int playerY) {
  if (myMap.getChar(x + vx, y + vy) == BORDER ||
      (x + vx == playerX && y + vy == playerY) ||
      x + vx > myMap.screenWidth() - 1)
    return false;
  return true;
}

void Monster::randomizeVelocity() {
  if (headOrTail() > 0) {
    vx = 0;
    vy = headOrTail();
  } else {
    vx = headOrTail();
    vy = 0;
  }
}

int Monster::getX() { return x; }

int Monster::getY() { return y; }

int Monster::getAttack() { return attack; }

int Monster::getHealth() { return health; }

char Monster::getTile() { return tile; }

void Monster::setHealth(int a) { health = a; }

std::vector<Monster> initalizeMonsters(int how_many, Map myMap) {
  std::vector<Monster> monsters;
  int pos[2] = {};

  // 1/2 goblins
  int number_of_goblins = (int)how_many / 2;

  for (int i = 0; i < number_of_goblins; i++) {
    giveValidPositions(pos, myMap);
    monsters.push_back(Monster(pos[0], pos[1], GOBLIN));
  }

  // 1/3 orcs
  int number_of_orcs = (int)how_many / 3;

  for (int i = 0; i < number_of_orcs; i++) {
    giveValidPositions(pos, myMap);
    monsters.push_back(Monster(pos[0], pos[1], ORC));
  }

  // 1/6 trolls
  int number_of_trolls = (int)how_many / 6;

  for (int i = 0; i < number_of_trolls; i++) {
    giveValidPositions(pos, myMap);
    monsters.push_back(Monster(pos[0], pos[1], TROLL));
  }

  // rest dragons
  int number_of_dragons = 0;

  while (number_of_goblins + number_of_orcs + number_of_trolls +
             number_of_dragons <
         how_many) {

    giveValidPositions(pos, myMap);
    monsters.push_back(Monster(pos[0], pos[1], DRAGON));
    number_of_dragons++;
  }

  return monsters;
}
