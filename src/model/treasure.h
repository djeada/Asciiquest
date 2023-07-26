#ifndef _TREASURE_H
#define _TREASURE_H

#include "entity.h"

class Treasure : public Entity {
private:
  int value;

public:
  Treasure();
  explicit Treasure(const Point &_position, int _value);

  int getValue() const;
  void setValue(int _value);

  std::string toString() const override;
};

#endif
