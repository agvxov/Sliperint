#ifndef SLIPERINT_DISPLAYER_TK_HPP
#define SLIPERINT_DISPLAYER_TK_HPP

#include "Sliperint_displayer.hpp"
extern "C" {
#include "tk_display.h"
}

class Sliperint_displayer_tk : public Sliperint_displayer {
public:
    Sliperint_displayer_tk(Sliperint * lab) : Sliperint_displayer(lab) {
        tcl_setup(sliperint->vlayout()[0].size(), sliperint->vlayout().size(), this);
        tcl_loop();
    }

    void update(move_t m) {
        ;
    }
};

#define COMMA ,
#define C_GETTER(NAME, RETURN, ARGV, MEMBER) \
extern "C" \
RETURN get_ ## NAME(void * d ARGV) { \
    return ((Sliperint_displayer*)d)->sliperint->MEMBER; \
}

C_GETTER(vwall, wall_t, COMMA int x COMMA int y, vlayout()[y][x])

C_GETTER(hwall, wall_t, COMMA int x COMMA int y, hlayout()[y][x])

C_GETTER(player_x, int, , player.position.x)

C_GETTER(player_y, int, , player.position.y)

C_GETTER(goal_x, int, , goal.x)

C_GETTER(goal_y, int, , goal.y)

#endif
