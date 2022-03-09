#ifndef _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_UTILS_H
#define _HOME_ADAM_MYSTERIOUS_DUNGEON_SRC_UTILS_H

struct Point {
  int x;
  int y;
  Point(int _x, int _y);
  Point();
  Point(const Point &other);
  auto operator==(const Point &p) const -> bool;
};

/*
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

                                   */
#endif
