#include "trap.h"
#include <algorithm>

Trap::Trap(const Point &position, TrapType type, CellType cellType)
    : Entity(position, cellType), trapType(type), state(TrapState::INACTIVE),
      activationCounter(0), cooldownCounter(0), activationInterval(10),
      cooldownDuration(5), damage(10) {}

Trap::~Trap() {}

void Trap::move(const Point &destination) {
  // Traps don't move once placed
  position = destination;
}

std::string Trap::toString() const {
  switch (trapType) {
    case TrapType::BLADE: return "Blade Trap";
    case TrapType::SPIKE: return "Spike Trap";
    case TrapType::ARROW: return "Arrow Trap";
    default: return "Trap";
  }
}

TrapState Trap::getState() const {
  return state;
}

TrapType Trap::getTrapType() const {
  return trapType;
}

int Trap::getDamage() const {
  return damage;
}

const std::vector<TrapProjectile>& Trap::getProjectiles() const {
  return projectiles;
}

void Trap::clearInactiveProjectiles() {
  projectiles.erase(
    std::remove_if(projectiles.begin(), projectiles.end(),
                   [](const TrapProjectile &proj) { return !proj.active; }),
    projectiles.end());
}

void Trap::deactivateProjectile(size_t index) {
  if (index < projectiles.size()) {
    projectiles[index].active = false;
  }
}

// BladeTrap implementation
BladeTrap::BladeTrap(const Point &position)
    : Trap(position, TrapType::BLADE, CellType::BLADE_TRAP),
      moveDirection(Direction::RIGHT), moveDistance(0), maxDistance(3) {
  activationInterval = 15;
  damage = 15;
}

BladeTrap::~BladeTrap() {}

void BladeTrap::update() {
  const int BLADE_TRAVEL_MULTIPLIER = 2; // Blades move out and back
  
  switch (state) {
    case TrapState::INACTIVE:
      activationCounter++;
      if (activationCounter >= activationInterval) {
        activate();
        activationCounter = 0;
      }
      break;
      
    case TrapState::ACTIVE: {
      // Move blades
      for (auto &proj : projectiles) {
        if (proj.active) {
          proj.position = proj.position + proj.direction;
          moveDistance++;
          
          // Deactivate if moved too far (travels out and back)
          if (moveDistance >= maxDistance * BLADE_TRAVEL_MULTIPLIER) {
            proj.active = false;
          }
        }
      }
      
      // Check if all projectiles are done
      bool allInactive = true;
      for (const auto &proj : projectiles) {
        if (proj.active) {
          allInactive = false;
          break;
        }
      }
      
      if (allInactive) {
        state = TrapState::COOLDOWN;
        cooldownCounter = 0;
        moveDistance = 0;
        clearInactiveProjectiles();
      }
      break;
    }
      
    case TrapState::COOLDOWN:
      cooldownCounter++;
      if (cooldownCounter >= cooldownDuration) {
        state = TrapState::INACTIVE;
        cooldownCounter = 0;
      }
      break;
  }
}

void BladeTrap::activate() {
  state = TrapState::ACTIVE;
  
  // Create blade projectile
  TrapProjectile blade;
  blade.position = position;
  blade.direction = moveDirection;
  blade.cellType = CellType::BLADE_PROJECTILE;
  blade.damage = damage;
  blade.active = true;
  
  projectiles.push_back(blade);
  
  // Alternate direction for next activation
  if (moveDirection == Direction::RIGHT) {
    moveDirection = Direction::LEFT;
  } else {
    moveDirection = Direction::RIGHT;
  }
}

std::string BladeTrap::toString() const {
  return "Blade Trap";
}

// SpikeTrap implementation
SpikeTrap::SpikeTrap(const Point &position)
    : Trap(position, TrapType::SPIKE, CellType::SPIKE_TRAP), extended(false) {
  activationInterval = 8;
  damage = 20;
}

SpikeTrap::~SpikeTrap() {}

void SpikeTrap::update() {
  switch (state) {
    case TrapState::INACTIVE:
      activationCounter++;
      if (activationCounter >= activationInterval) {
        activate();
        activationCounter = 0;
      }
      break;
      
    case TrapState::ACTIVE:
      // Spikes stay extended for a moment
      activationCounter++;
      if (activationCounter >= 3) {
        state = TrapState::COOLDOWN;
        cooldownCounter = 0;
        activationCounter = 0;
        extended = false;
        clearInactiveProjectiles();
      }
      break;
      
    case TrapState::COOLDOWN:
      cooldownCounter++;
      if (cooldownCounter >= cooldownDuration) {
        state = TrapState::INACTIVE;
        cooldownCounter = 0;
      }
      break;
  }
}

void SpikeTrap::activate() {
  state = TrapState::ACTIVE;
  extended = true;
  
  // Create spike projectile at trap position
  TrapProjectile spike;
  spike.position = position;
  spike.direction = Point(0, 0);
  spike.cellType = CellType::SPIKE_PROJECTILE;
  spike.damage = damage;
  spike.active = true;
  
  projectiles.push_back(spike);
}

std::string SpikeTrap::toString() const {
  return "Spike Trap";
}

// ArrowTrap implementation
ArrowTrap::ArrowTrap(const Point &position, Point direction)
    : Trap(position, TrapType::ARROW, CellType::ARROW_TRAP),
      shootDirection(direction) {
  activationInterval = 12;
  damage = 12;
}

ArrowTrap::~ArrowTrap() {}

void ArrowTrap::update() {
  switch (state) {
    case TrapState::INACTIVE:
      activationCounter++;
      if (activationCounter >= activationInterval) {
        activate();
        activationCounter = 0;
      }
      break;
      
    case TrapState::ACTIVE:
      // Move arrows
      for (auto &proj : projectiles) {
        if (proj.active) {
          proj.position = proj.position + proj.direction;
        }
      }
      
      // Arrows continue until they hit something or go off map
      // Model will handle collision detection and deactivation
      
      // Check if update cycle is complete
      activationCounter++;
      if (activationCounter >= 10) {
        state = TrapState::COOLDOWN;
        cooldownCounter = 0;
        activationCounter = 0;
      }
      break;
      
    case TrapState::COOLDOWN:
      cooldownCounter++;
      if (cooldownCounter >= cooldownDuration) {
        state = TrapState::INACTIVE;
        cooldownCounter = 0;
        clearInactiveProjectiles();
      }
      break;
  }
}

void ArrowTrap::activate() {
  state = TrapState::ACTIVE;
  
  // Create arrow projectile
  TrapProjectile arrow;
  arrow.position = position + shootDirection;
  arrow.direction = shootDirection;
  arrow.cellType = CellType::ARROW_PROJECTILE;
  arrow.damage = damage;
  arrow.active = true;
  
  projectiles.push_back(arrow);
}

std::string ArrowTrap::toString() const {
  return "Arrow Trap";
}
