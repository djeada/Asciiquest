#ifndef SPELL_H
#define SPELL_H

#include "utils/game_settings.h"
#include "utils/point.h"
#include <string>

enum class SpellType {
  FIRE,
  ICE,
  HEAL,
  LIGHTNING,
  SHIELD
};

class Spell {
protected:
  SpellType type;
  int damage;
  int manaCost;
  int range;
  char visual;
  std::string name;
  
public:
  Spell(SpellType _type, int _damage, int _manaCost, int _range, char _visual, const std::string& _name);
  virtual ~Spell();
  
  SpellType getType() const;
  int getDamage() const;
  int getManaCost() const;
  int getRange() const;
  char getVisual() const;
  std::string getName() const;
  
  virtual CellType getProjectileType() const = 0;
};

class FireSpell : public Spell {
public:
  FireSpell();
  CellType getProjectileType() const override;
};

class IceSpell : public Spell {
public:
  IceSpell();
  CellType getProjectileType() const override;
};

class HealSpell : public Spell {
public:
  HealSpell();
  CellType getProjectileType() const override;
};

class LightningSpell : public Spell {
public:
  LightningSpell();
  CellType getProjectileType() const override;
};

class ShieldSpell : public Spell {
public:
  ShieldSpell();
  CellType getProjectileType() const override;
};

#endif // SPELL_H
