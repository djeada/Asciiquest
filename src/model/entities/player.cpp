#include "player.h"
#include "utils/global_config.h"
#include <cmath>

Player::Player()
    : MovableEntity(CellType::PLAYER,
                    GlobalConfig::getInstance().getConfig<int>("PlayerHealth"),
                    GlobalConfig::getInstance().getConfig<int>("PlayerDamage")),
      level(1), exp(0) {}

Player::~Player() {}

void Player::levelUp() {
  level++;
  exp = 0;
  health = getMaxHealth();

  // we may need to level up multiple times if the exp was so great
  if (isLevelUp()) {
    levelUp();
  }
}

auto Player::getMaxHealth() const -> int {
  return GlobalConfig::getInstance().getConfig<int>("PlayerHealth") *
         pow(1.1, level - 1);
}

auto Player::expToNextLevel() const -> int {
  return level * 100 * pow(1.25, level - 1);
}

auto Player::isLevelUp() const -> bool { return exp >= expToNextLevel(); }

void Player::addExperience(int _exp) {
  exp += _exp;
  while (isLevelUp()) {
    levelUp();
  }
}

void Player::heal(int _health) {
  health += _health;
  if (health > getMaxHealth()) { // health should not exceed max health
    health = getMaxHealth();
  }
}

void Player::increaseStrength(int _strength) { strength += _strength; }

auto Player::toString() const -> std::string { return "Player"; }
