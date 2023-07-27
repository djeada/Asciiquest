#ifndef _RENDERER_DATA_H
#define _RENDERER_DATA_H

#include "utils/game_settings.h"
#include "utils/info_deque.h"
#include "utils/point.h"
#include <string>
#include <unordered_map>
#include <vector>

struct RendererData {
  /*
RendererData holds references to the original objects, so changes to the objects
in RendererData will affect the original objects, and vice versa.
*/
  std::vector<std::vector<CellType>> &grid;
  InfoDeque &messageQueue;
  std::unordered_map<std::string, std::string> &stats;
  Point &playerPosition;

  RendererData(std::vector<std::vector<CellType>> &_grid,
               InfoDeque &_messageQueue,
               std::unordered_map<std::string, std::string> &_stats,
               Point &_playerPosition

               )
      : grid(_grid), messageQueue(_messageQueue), stats(_stats),
        playerPosition(_playerPosition) {}
};

#endif
