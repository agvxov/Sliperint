#ifndef SLIPERINTH_HPP
#define SLIPERINTH_HPP

#include <assert.h>
#include <vector>
#include <memory>
#include "atom.h"
#include "player.hpp"

class Sliperinth {
public:
    coord_t goal = {0, 0};
    player_t player = {
        .position = {0, 0},
    };

    /* y1 [x1 x2 x3 x4]
     * y2 [x1 x2 x3 x4]
     * y3 [x1 x2 x3 x4]
     */
    std::vector<std::vector<wall_t>> vlayout;

    /* x1 [y1 y2 y3 y4]
     * x2 [y1 y2 y3 y4]
     * x3 [y1 y2 y3 y4]
     */
    std::vector<std::vector<wall_t>> hlayout;

    void validate_coord(coord_t c) {    // XXX
        if (c.y >= vlayout.size()) {
            vlayout.resize(c.y + 1);
            hlayout.resize(c.y + 1);
        }
        if (c.x >= vlayout[c.y].size()) {
            vlayout[c.y].resize(c.x + 1, false);
            hlayout[c.y].resize(c.x + 1, false);
        }
    }

    void set_vwalls(point_t y, std::vector<wall_t> w) {
        validate_coord(
            (coord_t){ .x = 0, .y = y, }
        );
        vlayout[y] = w;
    }

    void set_hwalls(point_t y, std::vector<wall_t> w) {
        validate_coord(
            (coord_t){ .x = 0, .y = y, }
        );
        hlayout[y] = w;
    }
};

class Sliperinth_builder {
    Sliperinth * sliperinth = new Sliperinth();
    
    int hwal_counter = 0;
    int vwal_counter = 0;
    
public:
    Sliperinth_builder * add_hwalls(std::vector<wall_t> w) {
        sliperinth->set_hwalls(hwal_counter++, w);
        return this;
    }

    Sliperinth_builder * add_vwalls(std::vector<wall_t> w) {
        sliperinth->set_vwalls(vwal_counter++, w);
        return this;
    }

    Sliperinth_builder * set_player(point_t x, point_t y) {
        sliperinth->player.position = {x, y};
        return this;
    }

    Sliperinth_builder * set_goal(point_t x, point_t y) {
        sliperinth->goal = {x, y};
        return this;
    }

    Sliperinth * build() {
        assert(sliperinth->hlayout.size() == sliperinth->vlayout.size());
        return this->sliperinth;
    }
};

#endif
