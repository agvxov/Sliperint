#include "sliperint_solver.hpp"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool is_valid(Sliperint * s) {
    return true;
}

bool is_operator(Sliperint * s, move_t m) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch"
    switch (m) {
        case MOVE_UP: {
            return not s->hlayout()
                        [s->player.position.y]
                        [s->player.position.x]
            ;
        };
        case MOVE_DOWN: {
            return not s->hlayout()
                        [s->player.position.y + 1]
                        [s->player.position.x]
            ;
        };
        case MOVE_LEFT: {
            return not s->vlayout()
                        [s->player.position.x]
                        [s->player.position.y]
            ;
        };
        case MOVE_RIGHT: {
            return not s->vlayout()
                        [s->player.position.x + 1]
                        [s->player.position.y]
            ;
        };
    }
    #pragma GCC diagnostic pop

    return false;
}

bool is_in_goal_state(Sliperint * s) {
    return s->goal.x == s->player.position.y
        && s->goal.y == s->player.position.y
    ;
}

void apply_operator(Sliperint * s, move_t m) {
    point_t * to_modify;
    auto wall = s->hlayout()[0].begin();
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch"
    switch (m) {
        case MOVE_UP:
        case MOVE_DOWN:
            to_modify = &(s->player.position.y);
            wall = s->hlayout()[s->player.position.y].begin() + s->player.position.x;
            break;
        case MOVE_LEFT:
        case MOVE_RIGHT:
            to_modify = &(s->player.position.x);
            wall = s->vlayout()[s->player.position.x].begin() + s->player.position.y;
            break;
    }

    int modify_with;
    switch (m) {
        case MOVE_UP:
        case MOVE_LEFT:
            modify_with = -1;
            break;
        case MOVE_DOWN:
        case MOVE_RIGHT:
            modify_with = 1;
            break;
    }
    #pragma GCC diagnostic pop

    while (not *(wall + *to_modify)) {
        *to_modify += modify_with;
    }
}

move_t inverse(move_t m) {
    switch (m) {
        case MOVE_UP:
            return MOVE_DOWN;
        case MOVE_DOWN:
            return MOVE_UP;
        case MOVE_RIGHT:
            return MOVE_LEFT;
        case MOVE_LEFT:
            return MOVE_RIGHT;
        case END_MOVE:
            return MOVE_LEFT;
    }
    return END_MOVE;
}

static
void _solve(Sliperint * const sliperint, Sliperint_displayer * const display, move_t last_direction) {
    display->update(last_direction);

    if (is_in_goal_state(sliperint)) {
        return;
    }

    Sliperint * s = new Sliperint(*sliperint);
    for (move_t m = (move_t)0; m < END_MOVE; m = (move_t)((int)m + 1)) {
        if (m == last_direction
        ||  m == inverse(last_direction)
        ||  not is_operator(s, m)) {
            continue;
        }

        apply_operator(s, m);

        last_direction = m;
        display->sliperint = s;

        _solve(s, display, last_direction);
    }
    delete s;
}

void solve(Sliperint * const sliperint, Sliperint_displayer * const display) {
    _solve(sliperint, display, END_MOVE);
}
