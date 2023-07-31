#ifndef _TREASURE_H
#define _TREASURE_H

#include "entity.h"
#include "utils/global_config.h"

enum class BonusType { Experience, Health, Strength };

class Treasure : public Entity {
private:
  int value;
  BonusType bonusType;
  int expirationCounter;

public:
  Treasure();
  explicit Treasure(const Point &_position, int _value, BonusType _bonusType,
                    int _expirationCounter);

  int getValue() const;
  void setValue(int _value);

  BonusType getBonusType() const;
  void setBonusType(BonusType _bonusType);

  bool isExpired() const;
  void decrementExpirationCounter();

  void move(const Point &destination) override;
  std::string toString() const override;
};

#endif
