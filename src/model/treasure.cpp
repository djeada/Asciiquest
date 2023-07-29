#include "treasure.h"
#include "utils/game_settings.h"
#include <random>

Treasure::Treasure() : Entity(), value(GlobalConfig::getInstance().getConfig<int>("BonusValue")), expirationCounter(GlobalConfig::getInstance().getConfig<int>("BonusExpirationCounter")) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 2);
    
    int randomType = distr(gen);
    switch(randomType) {
        case 0: 
            bonusType = BonusType::Experience;
            break;
        case 1: 
            bonusType = BonusType::Health;
            break;
        case 2: 
            bonusType = BonusType::Strength;
            break;
        default: 
            bonusType = BonusType::Experience;
    }

position = Point(-1, -1);
}

Treasure::Treasure(const Point &_position, int _value, BonusType _bonusType, int _expirationCounter)
    : Entity(_position, CellType::EMPTY), value(_value), bonusType(_bonusType), expirationCounter(_expirationCounter) {}

  void  Treasure::move(const Point &destination) {

if (position == Point(-1, -1)) {
position = destination;
}
}

int Treasure::getValue() const { return value; }

void Treasure::setValue(int _value) { value = _value; }

BonusType Treasure::getBonusType() const { return bonusType; }

void Treasure::setBonusType(BonusType _bonusType) { bonusType = _bonusType; }

bool Treasure::isExpired() const { return expirationCounter <= 0; }

void Treasure::decrementExpirationCounter() { --expirationCounter; }

std::string Treasure::toString() const { 
  switch (bonusType) {
    case BonusType::Experience: return "Treasure (Experience)";
    case BonusType::Health: return "Treasure (Health)";
    case BonusType::Strength: return "Treasure (Strength)";
    default: return "Treasure";
  }
}

