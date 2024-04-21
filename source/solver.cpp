#include "solver.hpp"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <random>
#include <chrono>

bool is_valid(Sliperint * s) {
    return true;
}

bool is_operator(Sliperint * s, move_t m) {
    switch (m) {
        case MOVE_UP: {
            return not s->hlayout()
                        [s->player.position.x]
                        [s->player.position.y]
            ;
        };
        case MOVE_DOWN: {
            return not s->hlayout()
                        [s->player.position.x]
                        [s->player.position.y + 1]
            ;
        };
        case MOVE_LEFT: {
            return not s->vlayout()
                        [s->player.position.y]
                        [s->player.position.x]
            ;
        };
        case MOVE_RIGHT: {
            return not s->vlayout()
                        [s->player.position.y]
                        [s->player.position.x + 1]
            ;
        };
        case END_MOVE: {
            assert(("END_MOVE was tested as a possible operator.", false));
        }
    }

    return false;
}

bool is_in_goal_state(Sliperint * s) {
    return s->goal.x == s->player.position.x
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
            wall = s->hlayout()[s->player.position.x].begin();
            break;
        case MOVE_LEFT:
        case MOVE_RIGHT:
            to_modify = &(s->player.position.x);
            wall = s->vlayout()[s->player.position.y].begin();
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
            ++wall;
            break;
    }
    #pragma GCC diagnostic pop

    while (not *(wall + *to_modify)) {
        *to_modify += modify_with;
    }
}

static
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
const move_t * random_move_order() {
    static move_t r[END_MOVE+1] = {MOVE_UP, MOVE_RIGHT, MOVE_LEFT, MOVE_DOWN, END_MOVE};
    //static std::default_random_engine generator(0);
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(r, r + END_MOVE, generator);
    return r;
}


static
bool _solve(Sliperint * const sliperint, Sliperint_displayer * const display, const move_t last_direction, const int recursion_counter) {
    display->update(last_direction);

    if (is_in_goal_state(sliperint)) {
        display->success(recursion_counter);
        return true;
    }

    move_t ms[END_MOVE+1];
    memcpy(ms, random_move_order(), (END_MOVE+1) * sizeof(move_t));
    for (move_t * mi = ms; *mi != END_MOVE; ++mi) {
        const auto &m = *mi;
        std::unique_ptr<Sliperint> scpy = std::make_unique<Sliperint>(*sliperint);
        Sliperint * s = scpy.get();

        if (m == last_direction
        ||  m == inverse(last_direction)
        ||  not is_operator(s, m)) {
            continue;
        }

        apply_operator(s, m);

        // XXX: theres a race condition here;
        // its possible that the displayer is keep trying to query the Sliperint
        //   after the function has returned at which point it has already been deleted.
        // the solution should be make the display store a copy
        //   and implement the operator = for copy and += for player/goal copy
        display->sliperint = s;

        sleep(1);
        if (_solve(s, display, m, recursion_counter + 1)) {
            return true;
        }
    }

    display->pop();
    return false;
}

void solve(Sliperint * const sliperint, Sliperint_displayer * const display) {
    _solve(sliperint, display, END_MOVE, 0);
}
