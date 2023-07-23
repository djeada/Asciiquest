#ifndef STATE_RENDERER_H
#define STATE_RENDERER_H

#include "utils/game_settings.h"
#include <string>
#include <unordered_map>
#include <vector>

class StateRenderer {
public:
  virtual ~StateRenderer() = default; // Ensure we have a virtual destructor

  virtual void draw(const std::vector<std::vector<CellType>> &grid,
                    const std::vector<std::string> &fightInfo,
                    const std::unordered_map<std::string, std::string>
                        &stats) = 0; // Pure virtual function
};

#endif // STATE_RENDERER_H
