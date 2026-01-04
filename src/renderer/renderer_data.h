#ifndef RENDERER_DATA_H
#define RENDERER_DATA_H

#include "utils/game_settings.h"
#include "utils/info_deque.h"
#include "utils/point.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declaration
class SpellEffect;

struct RendererData {
  /*
RendererData holds references to the original objects, so changes to the objects
in RendererData will affect the original objects, and vice versa.
*/
  std::vector<std::vector<CellType>> &grid;
  InfoDeque &messageQueue;
  std::unordered_map<std::string, std::string> &stats;
  Point &playerPosition;
  std::vector<std::shared_ptr<SpellEffect>> *spellEffects;

  RendererData(std::vector<std::vector<CellType>> &_grid,
               InfoDeque &_messageQueue,
               std::unordered_map<std::string, std::string> &_stats,
               Point &_playerPosition,
               std::vector<std::shared_ptr<SpellEffect>> *_spellEffects = nullptr
               )
      : grid(_grid), messageQueue(_messageQueue), stats(_stats),
        playerPosition(_playerPosition), spellEffects(_spellEffects) {}
};

#endif // RENDERER_DATA_H
