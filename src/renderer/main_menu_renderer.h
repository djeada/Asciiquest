#ifndef MAIN_MENU_RENDERER_H
#define MAIN_MENU_RENDERER_H

#include "state_renderer.h"

class MainMenuRenderer : public StateRenderer {
public:
  MainMenuRenderer();
  ~MainMenuRenderer() override;

  void draw() override;
};

#endif // MAIN_MENU_RENDERER_H
