#include "player.h"
#include "utils/global_config.h"
#include <cmath>

Player::Player()
    : MovableEntity(CellType::PLAYER,
                    GlobalConfig::getInstance().getConfig<int>("PlayerHealth"),
                    GlobalConfig::getInstance().getConfig<int>("PlayerDamage")),
      level(1), exp(0), mana(100), maxMana(100), lastDirection(1, 0) {
  initializeSpells();
}

Player::~Player() {}

void Player::initializeSpells() {
  spells.clear();
  spells.push_back(std::make_shared<FireSpell>());
  spells.push_back(std::make_shared<IceSpell>());
  spells.push_back(std::make_shared<HealSpell>());
  spells.push_back(std::make_shared<LightningSpell>());
  spells.push_back(std::make_shared<ShieldSpell>());
}

bool Player::castSpell(int spellIndex) {
  if (spellIndex < 0 || spellIndex >= static_cast<int>(spells.size())) {
    return false;
  }
  
  auto spell = spells[spellIndex];
  if (mana >= spell->getManaCost()) {
    mana -= spell->getManaCost();
    return true;
  }
  
  return false;
}

void Player::restoreMana(int amount) {
  mana += amount;
  if (mana > maxMana) {
    mana = maxMana;
  }
}

int Player::getMana() const {
  return mana;
}

int Player::getMaxMana() const {
  return maxMana;
}

std::shared_ptr<Spell> Player::getSpell(int index) const {
  if (index >= 0 && index < static_cast<int>(spells.size())) {
    return spells[index];
  }
  return nullptr;
}

int Player::getSpellCount() const {
  return static_cast<int>(spells.size());
}

void Player::levelUp() {
  level++;
  exp = 0;
  health = getMaxHealth();
  
  // Increase max mana on level up
  maxMana = 100 + (level - 1) * 10;
  mana = maxMana;

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

void Player::setLastDirection(const Point& direction) {
  // Only update if it's a non-zero direction
  if (direction.x != 0 || direction.y != 0) {
    lastDirection = direction;
  }
}

Point Player::getLastDirection() const {
  return lastDirection;
}

auto Player::toString() const -> std::string { return "Player"; }
