#include "controller/controller.h"
#include "model/model.h"
#include "renderer/renderer.h"

int main() {
  Model model;
  Renderer renderer;

  Controller controller(model, renderer);
  controller.run();

  return 0;
}
