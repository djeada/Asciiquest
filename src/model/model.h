#include "utils/direction.h"

#include "player.h"
#include "monster.h"
#include <vector>
#include <unordered_map>

class Model {

    // map: wall positions, start and end
    // player, has position, can move, can fight
    // enemies, has position, can move, can fight
    // tresure, has position, can move, can fight

    // handles AI and collision on high level, details implemented in lower level classes

    public:
        Model();
        void update();
        void fight(Monster& monster);
        void movePlayer(Direction direction);
        void loadMap();
        bool isGameOver();

	std::unordered_map<std::string, std::string> getPlayerStats();

    std::vector<std::string> fightInfo;
    std::vector<std::vector<CellType>> grid;
    Player player;
    std::vector<Monster> monsters;

    private: 
        void updateEntityPosition(Point oldPos, Point newPos);
	bool isWall(Point point);
bool isPlayer(Point point);
};
