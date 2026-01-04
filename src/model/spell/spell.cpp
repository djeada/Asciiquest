#include "spell.h"

Spell::Spell(SpellType _type, int _damage, int _manaCost, int _range, char _visual, const std::string& _name)
    : type(_type), damage(_damage), manaCost(_manaCost), range(_range), visual(_visual), name(_name) {}

Spell::~Spell() {}

SpellType Spell::getType() const { return type; }
int Spell::getDamage() const { return damage; }
int Spell::getManaCost() const { return manaCost; }
int Spell::getRange() const { return range; }
char Spell::getVisual() const { return visual; }
std::string Spell::getName() const { return name; }

// FireSpell - offensive elemental attack
FireSpell::FireSpell() : Spell(SpellType::FIRE, 30, 20, 8, '*', "Fireball") {}
CellType FireSpell::getProjectileType() const { return CellType::FIRE_PROJECTILE; }

// IceSpell - offensive elemental attack with slow effect
IceSpell::IceSpell() : Spell(SpellType::ICE, 25, 20, 8, '|', "Ice Shard") {}
CellType IceSpell::getProjectileType() const { return CellType::ICE_PROJECTILE; }

// HealSpell - self-healing spell
HealSpell::HealSpell() : Spell(SpellType::HEAL, 50, 30, 0, '+', "Heal") {}
CellType HealSpell::getProjectileType() const { return CellType::HEAL_EFFECT; }

// LightningSpell - powerful offensive spell
LightningSpell::LightningSpell() : Spell(SpellType::LIGHTNING, 40, 35, 10, '-', "Lightning Bolt") {}
CellType LightningSpell::getProjectileType() const { return CellType::LIGHTNING_PROJECTILE; }

// ShieldSpell - defensive barrier
ShieldSpell::ShieldSpell() : Spell(SpellType::SHIELD, 0, 25, 0, '#', "Shield") {}
CellType ShieldSpell::getProjectileType() const { return CellType::SHIELD_EFFECT; }
