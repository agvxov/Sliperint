#ifndef TK_ICE_LABIRINTH_DISPLAYER_HPP
#define TK_ICE_LABIRINTH_DISPLAYER_HPP

#include "Sliperinth_displayer.hpp"
extern "C" {
#include "tk_display.h"
}

class Sliperinth_displayer_tk : public Sliperinth_displayer {
public:
    Sliperinth_displayer_tk(Sliperinth * lab) : Sliperinth_displayer(lab) {
        tcl_setup(sliperinth.vlayout[0].size(), sliperinth.vlayout.size(), &(this->sliperinth));
        tcl_loop();
    }

    void update() {
        ;
    }
};

extern "C"
wall_t get_vwall(void * lab, int x, int y) {
    return ((Sliperinth*)lab)->vlayout[y][x];
}

extern "C"
wall_t get_hwall(void * lab, int x, int y) {
    return ((Sliperinth*)lab)->hlayout[y][x];
}

extern "C"
int get_player_x(void * lab) {
    return ((Sliperinth*)lab)->player.position.x;
}

extern "C"
int get_player_y(void * lab) {
    return ((Sliperinth*)lab)->player.position.y;
}

extern "C"
int get_goal_x(void * lab) {
    return ((Sliperinth*)lab)->goal.x;
}

extern "C"
int get_goal_y(void * lab) {
    return ((Sliperinth*)lab)->goal.y;
}

#endif
