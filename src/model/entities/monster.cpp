#include "monster.h"
#include "algorithms/a_star.h"
#include "utils/global_config.h"
#include <chrono>
#include <future>
#include <random>

std::unordered_map<CellType, int> monsterExpMap = {{CellType::GOBLIN, 100},
                                                   {CellType::ORC, 200},
                                                   {CellType::DRAGON, 300},
                                                   {CellType::TROLL, 400},
                                                   {CellType::SKELETON, 150}};

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

Goblin::Goblin(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player)
    : Monster(CellType::GOBLIN,
              GlobalConfig::getInstance().getConfig<int>("GoblinHealth"),
              GlobalConfig::getInstance().getConfig<int>("GoblinDamage")),
      map(std::move(_map)), player(std::move(_player)),
      followRange(GlobalConfig::getInstance().getConfig<int>("GoblinFollowRange")) {}

void Goblin::randomizeVelocity() {
  // Check if player is within follow range
  if (position.distance(player->position) <= followRange) {
    // Move towards player (simple direct approach)
    Point diff = player->position - position;
    velocity.x = (diff.x > 0) ? 1 : (diff.x < 0) ? -1 : 0;
    velocity.y = (diff.y > 0) ? 1 : (diff.y < 0) ? -1 : 0;
    
    // Ensure we have at least one component of movement
    if (velocity.x == 0 && velocity.y == 0) {
      Monster::randomizeVelocity();
    }
  } else {
    // Move randomly when player is out of range
    Monster::randomizeVelocity();
  }
}

void Goblin::move(const Point &destination) {
  MovableEntity::move(destination);
  randomizeVelocity();
}

std::string Goblin::toString() const { return "Goblin"; }
Orc::Orc(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player)
    : Monster(CellType::ORC,
              GlobalConfig::getInstance().getConfig<int>("OrcHealth"),
              GlobalConfig::getInstance().getConfig<int>("OrcDamage")),
      map(std::move(_map)), player(std::move(_player)),
      followRange(GlobalConfig::getInstance().getConfig<int>("OrcFollowRange")) {}

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
  if (position.distance(player->position) > followRange) {
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

Troll::Troll(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player)
    : Monster(CellType::TROLL,
              GlobalConfig::getInstance().getConfig<int>("TrollHealth"),
              GlobalConfig::getInstance().getConfig<int>("TrollDamage")),
      map(std::move(_map)), player(std::move(_player)),
      followRange(GlobalConfig::getInstance().getConfig<int>("TrollFollowRange")) {
  velocity = Point(1, 1);
}

void Troll::randomizeVelocity() {
  // Check if player is within follow range
  if (position.distance(player->position) <= followRange) {
    // Move towards player (simple direct approach)
    Point diff = player->position - position;
    velocity.x = (diff.x > 0) ? 1 : (diff.x < 0) ? -1 : 0;
    velocity.y = (diff.y > 0) ? 1 : (diff.y < 0) ? -1 : 0;
    
    // Ensure we have at least one component of movement
    if (velocity.x == 0 && velocity.y == 0) {
      Monster::randomizeVelocity();
    }
  } else {
    // Move randomly when player is out of range
    Monster::randomizeVelocity();
  }
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

Skeleton::Skeleton(std::shared_ptr<Map> _map, std::shared_ptr<Player> _player)
    : Monster(CellType::SKELETON,
              GlobalConfig::getInstance().getConfig<int>("SkeletonHealth"),
              GlobalConfig::getInstance().getConfig<int>("SkeletonDamage")),
      map(std::move(_map)), player(std::move(_player)),
      followRange(GlobalConfig::getInstance().getConfig<int>("SkeletonFollowRange")) {
  randomizeVelocity();
}

void Skeleton::randomizeVelocity() {
  static std::mt19937 gen = generateSeededRNG();
  std::uniform_int_distribution<> followChance(0, 99);
  
  // Check if player is within follow range
  if (position.distance(player->position) <= followRange) {
    // Move towards player with some randomness (erratic movement)
    Point diff = player->position - position;
    
    // 70% chance to move towards player, 30% random
    if (followChance(gen) < 70) {
      velocity.x = (diff.x > 0) ? 1 : (diff.x < 0) ? -1 : 0;
      velocity.y = (diff.y > 0) ? 1 : (diff.y < 0) ? -1 : 0;
      
      // Ensure we have at least one component of movement
      if (velocity.x == 0 && velocity.y == 0) {
        Monster::randomizeVelocity();
      }
    } else {
      Monster::randomizeVelocity();
    }
  } else {
    // Move randomly when player is out of range
    Monster::randomizeVelocity();
  }
}

void Skeleton::move(const Point &destination) {
  static std::mt19937 gen = generateSeededRNG();
  std::uniform_int_distribution<> changeChance(0, 1);
  
  MovableEntity::move(destination);
  // Skeletons have a 50% chance to change direction after each move
  if (changeChance(gen) == 0) {
    randomizeVelocity();
  }
}

std::string Skeleton::toString() const { return "Skeleton"; }
