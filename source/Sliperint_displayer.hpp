#ifndef SLIPERINT_DISPLAYER_HPP
#define SLIPERINT_DISPLAYER_HPP

#include "Sliperint.hpp"
#include <stdio.h>

class Sliperint_displayer {
public:
    Sliperint * sliperint = new Sliperint();
    Sliperint_displayer(Sliperint * lab) {
        *sliperint = *lab;
    }

    virtual void update(move_t m) = 0;

    virtual void pop() = 0;

    virtual void success(int i) = 0;

    int success_value = -1;
};

class Sliperint_displayer_basic : public Sliperint_displayer {
    std::vector<std::string> prefix;

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

    ~Sliperint_displayer_basic() {
        puts("");
        fflush(stdout);
    }

    void update(move_t m) {
        char * i;
        asprintf(&i, " %s {x: %ld; y: %ld}",
                move2arrow(m),
                sliperint->player.position.x,
                sliperint->player.position.y
        );
        prefix.push_back(i);
        fputs(i, stdout);
        fflush(stdout);
    }

    void pop() {
        puts("\n--");
        for (auto &i : prefix) {
            fputs(i.c_str(), stdout);
        }
        fflush(stdout);
        prefix.pop_back();
    }

    void success(int i) {
        printf("\n### Achived the goal state after %d steps of descent ###\n", i);
    }
};

#endif
