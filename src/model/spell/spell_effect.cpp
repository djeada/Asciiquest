#include "spell_effect.h"
#include <cmath>

SpellEffect::SpellEffect(std::shared_ptr<Spell> _spell, const Point& _origin, const Point& _direction)
    : spell(_spell), origin(_origin), currentPosition(_origin), direction(_direction),
      state(EffectState::CASTING), frameCount(0), maxFrames(20), travelDistance(0), explosionRadius(0) {
  
  // Normalize direction for consistent movement
  if (direction.x != 0 || direction.y != 0) {
    state = EffectState::TRAVELING;
  }
  
  target = origin + direction * spell->getRange();
}

SpellEffect::~SpellEffect() {}

void SpellEffect::update() {
  frameCount++;
  
  switch (state) {
    case EffectState::CASTING:
      if (frameCount >= 2) {
        if (spell->getType() == SpellType::HEAL || spell->getType() == SpellType::SHIELD) {
          state = EffectState::IMPACT;
          frameCount = 0;
        } else {
          state = EffectState::TRAVELING;
          frameCount = 0;
        }
      }
      break;
      
    case EffectState::TRAVELING:
      advanceTravelingState();
      break;
      
    case EffectState::IMPACT:
      advanceImpactState();
      break;
      
    case EffectState::EXPANDING:
      advanceExpandingState();
      break;
      
    case EffectState::FADING:
      if (frameCount >= 3) {
        state = EffectState::COMPLETE;
      }
      break;
      
    case EffectState::COMPLETE:
      break;
  }
}

void SpellEffect::advanceTravelingState() {
  // Move projectile forward
  if (travelDistance < spell->getRange()) {
    currentPosition = currentPosition + direction;
    travelDistance++;
  } else {
    state = EffectState::IMPACT;
    frameCount = 0;
  }
}

void SpellEffect::advanceImpactState() {
  if (frameCount >= 2) {
    // Start explosion expansion
    state = EffectState::EXPANDING;
    frameCount = 0;
    explosionRadius = 1;
  }
}

void SpellEffect::advanceExpandingState() {
  // Expand explosion
  if (explosionRadius < 3) {
    explosionRadius++;
  } else {
    state = EffectState::FADING;
    frameCount = 0;
  }
}

bool SpellEffect::isComplete() const {
  return state == EffectState::COMPLETE;
}

std::vector<EffectFrame> SpellEffect::getCurrentFrames() const {
  std::vector<EffectFrame> frames;
  
  switch (state) {
    case EffectState::CASTING: {
      // Simple visual at origin
      frames.push_back({origin, spell->getVisual(), spell->getProjectileType(), 0});
      break;
    }
    
    case EffectState::TRAVELING: {
      // Single projectile moving
      frames.push_back({currentPosition, spell->getVisual(), spell->getProjectileType(), 0});
      break;
    }
    
    case EffectState::IMPACT:
    case EffectState::EXPANDING: {
      // Expanding explosion effect
      // Create a pattern of characters around the impact point
      for (int dx = -explosionRadius; dx <= explosionRadius; dx++) {
        for (int dy = -explosionRadius; dy <= explosionRadius; dy++) {
          double distance = std::sqrt(dx * dx + dy * dy);
          if (distance <= explosionRadius) {
            Point pos = currentPosition + Point(dx, dy);
            frames.push_back({pos, spell->getVisual(), spell->getProjectileType(), explosionRadius});
          }
        }
      }
      break;
    }
    
    case EffectState::FADING: {
      // Smaller fading effect
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0) {
            Point pos = currentPosition + Point(dx, dy);
            frames.push_back({pos, spell->getVisual(), spell->getProjectileType(), 1});
          }
        }
      }
      break;
    }
    
    case EffectState::COMPLETE:
      break;
  }
  
  return frames;
}

Point SpellEffect::getCurrentPosition() const {
  return currentPosition;
}

EffectState SpellEffect::getState() const {
  return state;
}

std::shared_ptr<Spell> SpellEffect::getSpell() const {
  return spell;
}

int SpellEffect::getDamage() const {
  return spell->getDamage();
}
