#ifndef SLIPERINT_DISPLAYER_TK_INTERACTIVE_HPP
#define SLIPERINT_DISPLAYER_TK_INTERACTIVE_HPP

#include "Sliperint_displayer.hpp"
extern "C" {
#include "tk_display.h"
}

class Sliperint_displayer_tk_interactive : public Sliperint_displayer {
public:
    std::vector<Sliperint*> states;
    int position = 0;

    Sliperint_displayer_tk_interactive(Sliperint * lab) : Sliperint_displayer(lab) {
        states.push_back(new Sliperint(*lab));
        tcl_setup(sliperint->hlayout().size(), sliperint->vlayout().size() - 1, this);
        tcl_loop_interactive();
    }

    void update(move_t m) {
        states.push_back(new Sliperint(*(this->sliperint)));
    }

    void pop() {
        ;
    }

    void success(int i) {
        success_value = i;
        sliperint = states[0];
    }
};

extern "C"
void display_back(void * d) {
    Sliperint_displayer_tk_interactive &tk = *((Sliperint_displayer_tk_interactive*)d);
    if (tk.position > 0) {
        --tk.position;
        tk.sliperint = tk.states[tk.position];
    }
}

extern "C"
void display_next(void * d) {
    Sliperint_displayer_tk_interactive &tk = *((Sliperint_displayer_tk_interactive*)d);
    if (tk.position < tk.states.size()-1) {
        ++tk.position;
        tk.sliperint = tk.states[tk.position];
    }
}

#endif
