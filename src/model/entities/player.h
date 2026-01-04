#ifndef PLAYER_H
#define PLAYER_H

#include "movable_entity.h"
#include "model/spell/spell.h"
#include <memory>
#include <vector>

class Player : public MovableEntity {
private:
  bool isLevelUp() const;
  std::vector<std::shared_ptr<Spell>> spells;
  int mana;
  int maxMana;

public:
  Player();
  ~Player() override;

  void addExperience(int _exp);
  void heal(int _health);
  void increaseStrength(int _strength);

  void levelUp();
  int getMaxHealth() const;
  int expToNextLevel() const;
  
  // Spell system methods
  void initializeSpells();
  bool castSpell(int spellIndex);
  void restoreMana(int amount);
  int getMana() const;
  int getMaxMana() const;
  std::shared_ptr<Spell> getSpell(int index) const;
  int getSpellCount() const;

  std::string toString() const override;

  // data
  int level;
  int exp;
};

#endif // PLAYER_H
