#ifndef SLIPERINTH_HPP
#define SLIPERINTH_HPP

#include <vector>
#include "atom.h"
#include "player.hpp"

class Sliperinth {
public:
    coord_t goal;
    //player_t player((coord_t){0, 0});

    /* y1 [x1 x2 x2]
     * y2 [x1 x2 x2]
     * y3 [x1 x2 x2]
     */
    std::vector<std::vector<wall_t>> layout;

    void validate_coord(coord_t c) {
        if (c.y >= layout.size()) {
            layout.resize(c.y + 1);
        }
        if (c.x >= layout[c.y].size()) {
            layout[c.y].resize(c.x + 1, false);
        }
    }

    void add_wall(coord_t c, wall_t structure) {
        validate_coord(c);

        layout[c.y][c.x] = structure;
    }

    void set_walls(point_t y, std::vector<wall_t> w) {
        validate_coord(
            (coord_t){ .x = 0, .y = y, }
        );
        layout[y] = w;
    }
};

#endif
