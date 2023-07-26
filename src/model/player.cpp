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
  if (!isLevelUp()) {
    return;
  }

  level++;
  exp = 0;
  health = 1000 * pow(1.1, level - 1);

  if (isLevelUp()) {
    levelUp();
  }
}

auto Player::expToNextLevel() const -> int {
  return level * 100 * pow(1.25, level - 1);
}

auto Player::isLevelUp() const -> bool { return exp >= expToNextLevel(); }

void Player::setExp(int _exp) {
  exp = _exp;
  if (isLevelUp()) {
    levelUp();
  }
}
auto Player::getExp() const -> int { return exp; }

auto Player::toString() const -> std::string { return "Player"; }
