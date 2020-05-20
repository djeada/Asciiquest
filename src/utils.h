#ifndef utils_h
#define utils_h

#include "map.h"
#include <vector>

typedef struct pos {
  int x;
  int y;
} Position;

typedef struct node {
  bool isBorder;
  float distanceFromStart;
  float distanceTillEnd;
  Position cors;
  std::vector<node *> neighbours;
  struct node *parent;
} Node;

int randomFromRange(int start, int end);
int headOrTail();
void giveValidPositions(int (&myArray)[2], Map myMap);
std::vector<Position> findTheRoute(int playerX, int playerY, int monsterX,
                                   int MonsterY, Map &myMap);

#endif
