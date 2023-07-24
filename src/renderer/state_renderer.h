#ifndef STATE_RENDERER_H
#define STATE_RENDERER_H

#include "renderer_data.h"
#include "utils/game_settings.h"
#include <string>
#include <unordered_map>
#include <vector>

class StateRenderer {
public:
  virtual ~StateRenderer() = default; // Ensure we have a virtual destructor

  virtual void draw() = 0; // Pure virtual function
};

#endif // STATE_RENDERER_H
