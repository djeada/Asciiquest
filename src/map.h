#ifndef map_h
#define map_h

#include <string>
#include <vector>

class Map {
private:
  std::vector<std::string> rows;
  int currentLevel;

public:
  Map();
  void draw(int health, int level, int exp);
  char getChar(int x, int y);
  int screenWidth();
  int screenHeight();
  int getLevel();
  void loadTheMap(std::string path);
  void levelUp();
};

#endif
