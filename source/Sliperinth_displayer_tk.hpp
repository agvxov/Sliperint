#ifndef TK_ICE_LABIRINTH_DISPLAYER_HPP
#define TK_ICE_LABIRINTH_DISPLAYER_HPP

#include "Sliperinth_displayer.hpp"
extern "C" {
#include "tcl_display.h"
}

class Sliperinth_displayer_tk : public Sliperinth_displayer {
public:
    Sliperinth_displayer_tk(Sliperinth * lab) : Sliperinth_displayer(lab) {
        tcl_setup(sliperinth.layout[0].size(), sliperinth.layout.size(), &(this->sliperinth));
        tcl_loop();
    }

    void update() {
        ;
    }
};

extern "C"
wall_t get_wall(void * lab, int x, int y) {
    return ((Sliperinth*)lab)->layout[y][x];
}

#endif
