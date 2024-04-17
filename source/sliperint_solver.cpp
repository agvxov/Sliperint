#include "sliperint_solver.hpp"

bool is_valid(Sliperinth * s) {
    return true;
}

bool is_operator(Sliperinth * s, move_t m) {
    switch (m) {
        case MOVE_UP: {
            return s->vlayout
                        [s->player.position.y]
                        [s->player.position.x]
            ;
        };
        case MOVE_DOWN: {
            return s->vlayout
                        [s->player.position.y + 1]
                        [s->player.position.x]
            ;
        };
        case MOVE_LEFT: {
            return s->hlayout
                        [s->player.position.x]
                        [s->player.position.y]
            ;
        };
        case MOVE_RIGHT: {
            return s->hlayout
                        [s->player.position.x + 1]
                        [s->player.position.y]
            ;
        };
    }

    return false;
}

bool is_in_goal_state(Sliperinth * s) {
    return s->goal.x == s->player.position.y
        && s->goal.y == s->player.position.y
    ;
}

void apply_operator(Sliperinth * s, move_t m) {
    point_t * to_modify;
    auto wall = s->hlayout[0].begin();
    switch (m) {
        case MOVE_UP:
        case MOVE_DOWN:
            to_modify = &(s->player.position.y);
            wall = s->hlayout[s->player.position.x].begin() + s->player.position.y;
            break;
        case MOVE_LEFT:
        case MOVE_RIGHT:
            to_modify = &(s->player.position.x);
            wall = s->vlayout[s->player.position.x].begin() + s->player.position.y;
            break;
    }

    int modify_with;
    switch (m) {
        case MOVE_UP:
        case MOVE_LEFT:
            modify_with = -1;
        case MOVE_DOWN:
        case MOVE_RIGHT:
            modify_with = 1;
    }

    while (*(wall + modify_with)) {
        to_modify += modify_with;
    }
}

void solve(Sliperinth * sliperinth) {
    while (true) { ; }
}
