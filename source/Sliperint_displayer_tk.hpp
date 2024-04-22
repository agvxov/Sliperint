#ifndef SLIPERINT_DISPLAYER_TK_HPP
#define SLIPERINT_DISPLAYER_TK_HPP

#include "Sliperint_displayer.hpp"
extern "C" {
#include "tk_display.h"
}

class Sliperint_displayer_tk : public Sliperint_displayer {
public:
    int success_recursion = -1;

    Sliperint_displayer_tk(Sliperint * lab) : Sliperint_displayer(lab) {
        tcl_setup(sliperint->hlayout().size(), sliperint->vlayout().size() - 1, this);
        tcl_loop();
    }

    void update(move_t m) {
        sleep(1);
    }

    void pop() {
        ;
    }

    void success(int i) {
        success_recursion = i;
    }
};

#define COMMA ,
#define C_GETTER(NAME, RETURN, ARGV, MEMBER) \
extern "C" \
RETURN get_ ## NAME(void * d ARGV) { \
    return ((Sliperint_displayer*)d)->sliperint->MEMBER; \
}

C_GETTER(vwall, wall_t, COMMA int x COMMA int y, vlayout()[y][x])

C_GETTER(hwall, wall_t, COMMA int x COMMA int y, hlayout()[x][y])

C_GETTER(player_x, int, , player.position.x)

C_GETTER(player_y, int, , player.position.y)

C_GETTER(goal_x, int, , goal.x)

C_GETTER(goal_y, int, , goal.y)

extern "C"
const char * get_state(void * d) {
    static char * r;
    const Sliperint_displayer_tk &tk = *((Sliperint_displayer_tk*)d);
    if (tk.success_recursion == -1) {
        return "Searching...";
    } else {
        asprintf(&r, "Found the solution in %d steps of descend!", tk.success_recursion);
        return r;
    }
}

#endif
