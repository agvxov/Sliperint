#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "atom.h"

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT,
} move_t;

struct player_t {
    coord_t position;

    bool move(move_t direction) {
        return true;
    }

    player_t(coord_t pos) : position(pos) {
        ;
    }
};

#endif