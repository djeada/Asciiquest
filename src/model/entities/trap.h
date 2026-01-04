#ifndef TRAP_H
#define TRAP_H

#include "entity.h"
#include "utils/direction.h"
#include <memory>
#include <vector>

enum class TrapType {
  BLADE,
  SPIKE,
  ARROW
};

enum class TrapState {
  INACTIVE,
  ACTIVE,
  COOLDOWN
};

struct TrapProjectile {
  Point position;
  Point direction;
  CellType cellType;
  int damage;
  bool active;
};

class Trap : public Entity {
protected:
  TrapType trapType;
  TrapState state;
  int activationCounter;
  int cooldownCounter;
  int activationInterval;
  int cooldownDuration;
  int damage;
  std::vector<TrapProjectile> projectiles;

public:
  explicit Trap(const Point &position, TrapType type, CellType cellType);
  virtual ~Trap() override;
  
  virtual void update() = 0;
  virtual void activate() = 0;
  
  void move(const Point &destination) override;
  std::string toString() const override;
  
  TrapState getState() const;
  TrapType getTrapType() const;
  int getDamage() const;
  const std::vector<TrapProjectile>& getProjectiles() const;
  void clearInactiveProjectiles();
};

class BladeTrap : public Trap {
private:
  Point moveDirection;
  int moveDistance;
  int maxDistance;
  
public:
  explicit BladeTrap(const Point &position);
  ~BladeTrap() override;
  
  void update() override;
  void activate() override;
  std::string toString() const override;
};

class SpikeTrap : public Trap {
private:
  bool extended;
  
public:
  explicit SpikeTrap(const Point &position);
  ~SpikeTrap() override;
  
  void update() override;
  void activate() override;
  std::string toString() const override;
};

class ArrowTrap : public Trap {
private:
  Point shootDirection;
  
public:
  explicit ArrowTrap(const Point &position, Point direction);
  ~ArrowTrap() override;
  
  void update() override;
  void activate() override;
  std::string toString() const override;
};

#endif // TRAP_H
