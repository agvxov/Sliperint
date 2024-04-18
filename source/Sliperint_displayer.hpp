#ifndef SLIPERINT_DISPLAYER_HPP
#define SLIPERINT_DISPLAYER_HPP

#include "Sliperint.hpp"
#include <stdio.h>

class Sliperint_displayer {
public:
    Sliperint * sliperint;
    Sliperint_displayer(Sliperint * lab) : sliperint(lab) {
        ;
    }

    virtual void update(move_t m) = 0;
};

class Sliperint_displayer_basic : public Sliperint_displayer {
    const char * move2arrow(move_t m) {
        switch (m) {
            case MOVE_UP:
                return "^";
            case MOVE_DOWN:
                return "V";
            case MOVE_RIGHT:
                return ">";
            case MOVE_LEFT:
                return "<";
            case END_MOVE:
                return " ";
        }
        return "";
    }
public:
    Sliperint_displayer_basic(Sliperint * lab) : Sliperint_displayer(lab) {
        ;
    }

    void update(move_t m) {
        printf(" %s {x: %ld; y: %ld}",
                move2arrow(m),
                sliperint->player.position.x,
                sliperint->player.position.y
        );
        fflush(stdout);
    }
};

#endif
