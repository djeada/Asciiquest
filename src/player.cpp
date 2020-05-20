#include "player.h"
#include <cmath>
#include <ncurses.h>
#include <stdio.h>

Player::Player() {
  x = 1;
  y = 1;

  attack = 1;
  health = 10;

  level = 0;
  exp = 0;
  newMapLevelFlag = false;
}

Player::Player(int _x, int _y) {
  x = _x;
  y = _y;

  attack = 1;
  health = 10;

  level = 0;
  exp = 0;
  newMapLevelFlag = false;
}

void Player::draw() {

  if (health > 0) {
    attron(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(y, x, PLAYER_TILE);
    attroff(COLOR_PAIR(PLAYER_COLOR));
    move(y, x);
  }

  else {
    mvprintw(y, x, DEAD_PLAYER_TILE);
    move(y, x);
  }
}

void Player::movePlayer(int dx, int dy) {
  if (checkNoColisions(dx, dy)) {
    x += dx;
    y += dy;
  }
  if (x == myMap.screenWidth() - 1) {
    myMap.levelUp();
    if (myMap.getLevel() != number_of_levels) {
      newMapLevelFlag = true;
      x = 1;
      y = 1;
    }
  }
}

bool Player::checkNoColisions(int dx, int dy) {
  if (myMap.getChar(x + dx, y + dy) == BORDER)
    return false;
  return true;
}

void Player::handleInput(char input) {

  if (health > 0) {
    switch (input) {
    case 'w':
    case 'W':
      movePlayer(0, -1);
      break;

    case 'a':
    case 'A':
      movePlayer(-1, 0);
      break;

    case 's':
    case 'S':
      movePlayer(0, 1);
      break;

    case 'd':
    case 'D':
      movePlayer(1, 0);
      break;

    default:
      printf("Default \n");
      break;
    }
  }
}

bool isInRange(int x1, int y1, int x2, int y2) {
  float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
  if (dist < 2)
    return true;

  return false;
}

void Player::fight(std::vector<Monster> &monsters) {

  auto i = std::begin(monsters);

  while (i != std::end(monsters)) {
    if (isInRange(x, y, i->getX(), i->getY())) {
      if (headOrTail()) {
        i->setHealth(i->getHealth() - attack);
        if (i->getHealth() < 0) {
          increaseExp(i->getTile());
          i = monsters.erase(i);
          if (levelUp()) {
            health = 10 * (level + 1);
          }
          break;
        }
      }
      if (headOrTail()) {
        health -= i->getAttack();
        if (health < 0)
          break;
      }
    } else
      ++i;
  }
}

void Player::checkTreasure(std::vector<Treasure> &treasures) {

  auto i = std::begin(treasures);

  while (i != std::end(treasures)) {
    if (isInRange(x, y, i->getX(), i->getY())) {
      attack += i->getBonusAttack();
      exp += i->getBonusExp();
      health += i->getBonusHealth();
      i = treasures.erase(i);
    } else
      ++i;
  }
}

void Player::increaseExp(char tile) {
  switch (tile) {
  case GOBLIN:
    exp += 3;
    break;

  case ORC:
    exp += 5;
    break;

  case TROLL:
    exp += 10;
    break;

  case DRAGON:
    exp += 50;
    break;

  default:
    break;
  }
}

bool Player::levelUp() {
  if (exp >= level * 10) {
    while (exp >= level * 10) {
      exp -= level * 10;
      level++;
    }
    return true;
  }
  return false;
}

int Player::getX() { return x; }

int Player::getY() { return y; }

int Player::getHealth() { return health; }

int Player::getExp() { return exp; }

int Player::getLevel() { return level; }

void Player::turnOffMapLevelFlag() { newMapLevelFlag = false; }

bool Player::newMapLevel() { return newMapLevelFlag; }

Map Player::getMap() { return myMap; }
