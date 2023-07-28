#include "monster.h"
#include "algorithms/a_star.h"
#include "utils/global_config.h"
#include <chrono>
#include <future>
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
Orc::Orc(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player)
    : Monster(CellType::ORC,
              GlobalConfig::getInstance().getConfig<int>("OrcHealth"),
              GlobalConfig::getInstance().getConfig<int>("OrcDamage")),
      map(std::move(_map)), player(std::move(_player)) {}

void Orc::move(const Point &destination) {
  std::lock_guard<std::mutex> lock(mutex);
  MovableEntity::move(destination);

  if (position.distance(player->position) < 5) {
    randomizeVelocity();
  }
}

Point Orc::getVelocity() {
  std::lock_guard<std::mutex> lock(mutex);
  if (!path.empty()) {
    auto newPos = path.front();
    path.pop_front();
    return newPos - position;
  }
  randomizeVelocity();
  return velocity;
}

std::string Orc::toString() const { return "Orc"; }

void Orc::randomizeVelocity() {
  if (position.distance(player->position) > 10) {
    return;
  }

  auto isNavigable = [](const CellType &cell) {
    return cell == CellType::EMPTY || cell == CellType::PLAYER;
  };

  auto futurePath =
      std::async(std::launch::async, [&, playerPosition = player->position]() {
        AStar<CellType> aStar(map->grid, position, playerPosition, isNavigable);
        return aStar.getPath();
      });

  try {
    path = futurePath.get();
    if (!path.empty()) {
      path.pop_front();
    }
  } catch (const std::system_error &e) {
    // Handle the error
    std::cerr << "Error occurred: " << e.what() << '\n';
  }
}

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
