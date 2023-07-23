#include "renderer.h"



std::unordered_map<CellType, std::pair<char, int> > cellTypeToCharColor = {
    {CellType::EMPTY, {' ', COLOR_BLACK}},
    {CellType::WALL, {'#', COLOR_BLUE}},
    {CellType::PLAYER, {'@', COLOR_RED}}

};

Renderer::Renderer() {
            initscr();
            start_color();
            curs_set(0);
            noecho();

            init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
            init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_WHITE);
            init_pair(COLOR_RED, COLOR_RED, COLOR_WHITE);

        }

Renderer::~Renderer() {
            endwin();
        }


        void Renderer::drawBoard(const std::vector<std::vector<CellType>>& grid) {
            for (int y = 0; y < grid.size(); y++) {
                for(int x =0; x < grid[y].size(); x++) {
                    auto cell_type = grid[y][x];
                    const auto& [ch, color] = cellTypeToCharColor[cell_type];
                    attron(COLOR_PAIR(color));
                    mvaddch(y, x, ch);
                    attroff(COLOR_PAIR(color));
                }
            }
            refresh();
        }


	void Renderer::drawFightInfo(const std::vector<std::string>& info){
}
	void Renderer::drawStats(const std::unordered_map<std::string, std::string>& info){

}
