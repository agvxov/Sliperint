#ifndef SLIPERINTH_DISPLAYER_HPP
#define SLIPERINTH_DISPLAYER_HPP

#include "Sliperinth.hpp"

class Sliperinth_displayer {
public:
    Sliperinth &sliperinth;
    Sliperinth_displayer(Sliperinth * lab) : sliperinth(*lab) {
        ;
    }

    virtual void update() = 0;
};

#endif
