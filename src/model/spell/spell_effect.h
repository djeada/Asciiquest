#ifndef SPELL_EFFECT_H
#define SPELL_EFFECT_H

#include "spell.h"
#include "utils/point.h"
#include <memory>
#include <vector>

enum class EffectState {
  CASTING,
  TRAVELING,
  IMPACT,
  EXPANDING,
  FADING,
  COMPLETE
};

struct EffectFrame {
  Point position;
  char visual;
  CellType cellType;
  int radius; // For explosion effects
};

class SpellEffect {
private:
  std::shared_ptr<Spell> spell;
  Point origin;
  Point target;
  Point currentPosition;
  Point direction;
  EffectState state;
  int frameCount;
  int maxFrames;
  int travelDistance;
  int explosionRadius;
  
  void advanceTravelingState();
  void advanceImpactState();
  void advanceExpandingState();
  
public:
  SpellEffect(std::shared_ptr<Spell> _spell, const Point& _origin, const Point& _direction);
  ~SpellEffect();
  
  void update();
  bool isComplete() const;
  
  std::vector<EffectFrame> getCurrentFrames() const;
  
  Point getCurrentPosition() const;
  EffectState getState() const;
  std::shared_ptr<Spell> getSpell() const;
  int getDamage() const;
};

#endif // SPELL_EFFECT_H
