#include "monster.h"
#include "utils/global_config.h"
#include <chrono>
#include <random>

std::unordered_map<CellType, int> monsterExpMap = {{CellType::GOBLIN, 100},
                                                   {CellType::ORC, 200},
                                                   {CellType::DRAGON, 300},
                                                   {CellType::TROLL, 400}};

std::mt19937 generateSeededRNG() {
  std::random_device rd;
  std::mt19937 gen(rd());
  return gen;
}

Monster::Monster(CellType cellType, int _health, int _attack)
    : MovableEntity(cellType, _health, _attack) {}

void Monster::randomizeVelocity() {
  static std::mt19937 gen = generateSeededRNG();
  std::uniform_int_distribution<> distrib(-1, 1);

  do {
    velocity.x = distrib(gen);
    velocity.y = distrib(gen);
  } while (velocity.x == 0 && velocity.y == 0);
}

Goblin::Goblin()
    : Monster(CellType::GOBLIN,
              GlobalConfig::getInstance().getConfig<int>("GoblinHealth"),
              GlobalConfig::getInstance().getConfig<int>("GoblinDamage")) {}

void Goblin::move(const Point &destination) {
  MovableEntity::move(destination);
  Monster::randomizeVelocity();
}

std::string Goblin::toString() const { return "Goblin"; }

Orc::Orc()
    : Monster(CellType::ORC,
              GlobalConfig::getInstance().getConfig<int>("OrcHealth"),
              GlobalConfig::getInstance().getConfig<int>("OrcDamage")) {}

void Orc::move(const Point &destination) {
  if (!path.empty()) {
    position = path.front();
    path.pop_front();
  }
}

std::string Orc::toString() const { return "Orc"; }

void Orc::setPath(const std::deque<Point> &path) { this->path = path; }

bool Orc::isPathEmpty() const { return path.empty(); }

Troll::Troll()
    : Monster(CellType::TROLL,
              GlobalConfig::getInstance().getConfig<int>("TrollHealth"),
              GlobalConfig::getInstance().getConfig<int>("TrollDamage")) {
  velocity = Point(1, 1);
}

void Troll::move(const Point &destination) {
  MovableEntity::move(destination);
  randomizeVelocity();
}

std::string Troll::toString() const { return "Troll"; }

Dragon::Dragon()
    : Monster(CellType::DRAGON,
              GlobalConfig::getInstance().getConfig<int>("DragonHealth"),
              GlobalConfig::getInstance().getConfig<int>("DragonDamage")) {
  velocity = Point(0, 0);
}

void Dragon::move(const Point &destination) {
  // Dragon does not move.
}

void Dragon::randomizeVelocity() {
  // Dragon does not move.
}

std::string Dragon::toString() const { return "Dragon"; }
