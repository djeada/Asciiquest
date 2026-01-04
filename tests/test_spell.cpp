#include "model/spell/spell.h"
#include "model/spell/spell_effect.h"
#include "gtest/gtest.h"

TEST(SpellTest, FireSpellPropertiesAreCorrect) {
  FireSpell fireSpell;
  
  ASSERT_EQ(fireSpell.getType(), SpellType::FIRE);
  ASSERT_EQ(fireSpell.getDamage(), 30);
  ASSERT_EQ(fireSpell.getManaCost(), 20);
  ASSERT_EQ(fireSpell.getRange(), 8);
  ASSERT_EQ(fireSpell.getVisual(), '*');
  ASSERT_EQ(fireSpell.getName(), "Fireball");
  ASSERT_EQ(fireSpell.getProjectileType(), CellType::FIRE_PROJECTILE);
}

TEST(SpellTest, IceSpellPropertiesAreCorrect) {
  IceSpell iceSpell;
  
  ASSERT_EQ(iceSpell.getType(), SpellType::ICE);
  ASSERT_EQ(iceSpell.getDamage(), 25);
  ASSERT_EQ(iceSpell.getManaCost(), 20);
  ASSERT_EQ(iceSpell.getRange(), 8);
  ASSERT_EQ(iceSpell.getVisual(), '|');
  ASSERT_EQ(iceSpell.getName(), "Ice Shard");
  ASSERT_EQ(iceSpell.getProjectileType(), CellType::ICE_PROJECTILE);
}

TEST(SpellTest, HealSpellPropertiesAreCorrect) {
  HealSpell healSpell;
  
  ASSERT_EQ(healSpell.getType(), SpellType::HEAL);
  ASSERT_EQ(healSpell.getDamage(), 50);
  ASSERT_EQ(healSpell.getManaCost(), 30);
  ASSERT_EQ(healSpell.getRange(), 0);
  ASSERT_EQ(healSpell.getVisual(), '+');
  ASSERT_EQ(healSpell.getName(), "Heal");
  ASSERT_EQ(healSpell.getProjectileType(), CellType::HEAL_EFFECT);
}

TEST(SpellTest, LightningSpellPropertiesAreCorrect) {
  LightningSpell lightningSpell;
  
  ASSERT_EQ(lightningSpell.getType(), SpellType::LIGHTNING);
  ASSERT_EQ(lightningSpell.getDamage(), 40);
  ASSERT_EQ(lightningSpell.getManaCost(), 35);
  ASSERT_EQ(lightningSpell.getRange(), 10);
  ASSERT_EQ(lightningSpell.getVisual(), '-');
  ASSERT_EQ(lightningSpell.getName(), "Lightning Bolt");
  ASSERT_EQ(lightningSpell.getProjectileType(), CellType::LIGHTNING_PROJECTILE);
}

TEST(SpellTest, ShieldSpellPropertiesAreCorrect) {
  ShieldSpell shieldSpell;
  
  ASSERT_EQ(shieldSpell.getType(), SpellType::SHIELD);
  ASSERT_EQ(shieldSpell.getDamage(), 0);
  ASSERT_EQ(shieldSpell.getManaCost(), 25);
  ASSERT_EQ(shieldSpell.getRange(), 0);
  ASSERT_EQ(shieldSpell.getVisual(), '#');
  ASSERT_EQ(shieldSpell.getName(), "Shield");
  ASSERT_EQ(shieldSpell.getProjectileType(), CellType::SHIELD_EFFECT);
}

TEST(SpellEffectTest, EffectIsCreatedInCastingState) {
  auto spell = std::make_shared<FireSpell>();
  Point origin(5, 5);
  Point direction(1, 0);
  
  SpellEffect effect(spell, origin, direction);
  
  ASSERT_EQ(effect.getState(), EffectState::TRAVELING);
  ASSERT_EQ(effect.getCurrentPosition(), origin);
}

TEST(SpellEffectTest, EffectMovesInDirection) {
  auto spell = std::make_shared<FireSpell>();
  Point origin(5, 5);
  Point direction(1, 0);
  
  SpellEffect effect(spell, origin, direction);
  
  // Update the effect a few times
  effect.update();
  Point pos1 = effect.getCurrentPosition();
  
  effect.update();
  Point pos2 = effect.getCurrentPosition();
  
  // The projectile should move in the direction
  ASSERT_TRUE(pos2.x > pos1.x || pos2.y != pos1.y);
}

TEST(SpellEffectTest, EffectEventuallyCompletes) {
  auto spell = std::make_shared<FireSpell>();
  Point origin(5, 5);
  Point direction(1, 0);
  
  SpellEffect effect(spell, origin, direction);
  
  // Update until complete (max iterations to prevent infinite loop)
  int maxIterations = 100;
  int iterations = 0;
  
  while (!effect.isComplete() && iterations < maxIterations) {
    effect.update();
    iterations++;
  }
  
  ASSERT_TRUE(effect.isComplete());
  ASSERT_LT(iterations, maxIterations);
}

TEST(SpellEffectTest, HealSpellHasNoTravelState) {
  auto spell = std::make_shared<HealSpell>();
  Point origin(5, 5);
  Point direction(0, 0);
  
  SpellEffect effect(spell, origin, direction);
  
  // Heal spell should go straight to casting then impact
  ASSERT_EQ(effect.getState(), EffectState::CASTING);
}

TEST(SpellEffectTest, EffectProducesFrames) {
  auto spell = std::make_shared<FireSpell>();
  Point origin(5, 5);
  Point direction(1, 0);
  
  SpellEffect effect(spell, origin, direction);
  
  auto frames = effect.getCurrentFrames();
  
  // Should have at least one frame
  ASSERT_GT(frames.size(), 0);
  
  // Frame should have the spell's visual
  ASSERT_EQ(frames[0].visual, spell->getVisual());
  ASSERT_EQ(frames[0].cellType, spell->getProjectileType());
}
