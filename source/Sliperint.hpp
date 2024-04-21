#ifndef SLIPERINT_HPP
#define SLIPERINT_HPP

#include <assert.h>
#include <vector>
#include <memory>
#include "atom.h"

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT,
    END_MOVE,
} move_t;

class Sliperint {
    /* y1 [x1 x2 x3 x4]
     * y2 [x1 x2 x3 x4]
     * y3 [x1 x2 x3 x4]
     */
    std::shared_ptr<std::vector<std::vector<wall_t>>> _vlayout;

    /* x1 [y1 y2 y3 y4]
     * x2 [y1 y2 y3 y4]
     * x3 [y1 y2 y3 y4]
     */
    std::shared_ptr<std::vector<std::vector<wall_t>>> _hlayout;

public:
    coord_t goal = {0, 0};

    struct {
        coord_t position;
    } player = {
        .position = {0, 0},
    };

    std::vector<std::vector<wall_t>> &vlayout() const {
        return *(this->_vlayout);
    }

    std::vector<std::vector<wall_t>> &hlayout() const {
        return *(this->_hlayout);
    }

    Sliperint() : _vlayout(new std::vector<std::vector<wall_t>>),
                   _hlayout(new std::vector<std::vector<wall_t>>) {
        ;
    }

    Sliperint(const Sliperint &s) : _vlayout(s._vlayout),
                                      _hlayout(s._hlayout),
                                      goal(s.goal),
                                      player(s.player) {
        ;
    }

    void validate_coord(coord_t c) {
        if (c.y >= vlayout().size()) {
            vlayout().resize(c.y + 1);
        }

        if (c.x >= hlayout().size()) {
            hlayout().resize(c.x + 1);
        }

        if (c.x >= vlayout()[c.y].size()) {
            vlayout()[c.y].resize(c.x + 1, false);
        }

        if (c.y >= hlayout()[c.x].size()) {
            hlayout()[c.x].resize(c.y + 1, false);
        }
    }

    void set_vwalls(point_t y, std::vector<wall_t> w) {
        validate_coord(
            (coord_t){ .x = 0, .y = y, }
        );
        vlayout()[y] = w;
    }

    void set_hwalls(point_t x, std::vector<wall_t> w) {
        validate_coord(
            (coord_t){ .x = x, .y = 0, }
        );
        hlayout()[x] = w;
    }
};

class Sliperint_builder {
    Sliperint * sliperint = new Sliperint();
    
    int hwal_counter = 0;
    int vwal_counter = 0;
    
public:
    Sliperint_builder * add_hwalls(std::vector<wall_t> w) {
        for (int i = 0; i < w.size(); i++) {
            sliperint->validate_coord((coord_t){i, hwal_counter});
            sliperint->hlayout()[i][hwal_counter] = w[i];
        }
        ++hwal_counter;
        return this;
    }

    Sliperint_builder * add_vwalls(std::vector<wall_t> w) {
        sliperint->set_vwalls(vwal_counter++, w);
        return this;
    }

    Sliperint_builder * set_player(point_t x, point_t y) {
        sliperint->player.position = {x, y};
        return this;
    }

    Sliperint_builder * set_goal(point_t x, point_t y) {
        sliperint->goal = {x, y};
        return this;
    }

    Sliperint * build() {
        //// Size match checking
        //// -- first coord
        //assert(sliperint->hlayout().size() == sliperint->vlayout()[0].size());
        //assert(sliperint->vlayout().size() == sliperint->hlayout()[0].size());
        //// -- last coord
        //assert(sliperint->hlayout()[0].size() == (*(sliperint->hlayout().end() - 1)).size());
        //assert(sliperint->vlayout()[0].size() == (*(sliperint->vlayout().end() - 1)).size());
        //// -- we seriously hope the Dev did not manage to make the middle differently dimensioned somehow

        return this->sliperint;
    }
};

#endif
