#include "solver.hpp"

#include <string.h>
#include <vector>
#include <tuple>
#include <queue>
#include <deque>
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
            assert(((void)"END_MOVE was tested as a possible operator.", false));
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
            return (move_t)-1;
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


/* Totally random search.
 *  While unlikely, it could start infinitly circling sooner or later
 *  crashing because of the bloated stack.
 */
[[ maybe_unused ]]
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

        *(display->sliperint) += *s;

        if (_solve(s, display, m, recursion_counter + 1)) {
            return true;
        }
    }

    display->pop();
    return false;
}


/* Random, but exhaustive.
 *  Directions are checked at random and no record of previously seen states is kept,
 *  but will look in all directions from the state its in.
 */
[[ maybe_unused ]]
static
bool _solve2(Sliperint * const sliperint, Sliperint_displayer * const display) {
    std::queue<std::tuple<Sliperint*, move_t>> branched_states;
    branched_states.push(std::make_tuple(sliperint, END_MOVE));
    int descent_counter = 0;

    display->update(END_MOVE);

    while (not branched_states.empty()) {
        Sliperint * current_sliperint = std::get<0>(branched_states.front());
        move_t last_direction = std::get<1>(branched_states.front());
        branched_states.pop();

        *(display->sliperint) += *current_sliperint;
        display->update(last_direction);

        if (is_in_goal_state(current_sliperint)) {
            display->success(descent_counter);
            return true;
        }

        ++descent_counter;

        move_t ms[END_MOVE+1];
        memcpy(ms, random_move_order(), (END_MOVE+1) * sizeof(move_t));
        for (move_t * mi = ms; *mi != END_MOVE; ++mi) {
            const auto &m = *mi;
            Sliperint * up_comming_sliperint = new Sliperint(*current_sliperint);

            if (m == last_direction
            ||  m == inverse(last_direction)
            ||  not is_operator(up_comming_sliperint, m)) {
                continue;
            }

            apply_operator(up_comming_sliperint, m);

            branched_states.push(std::make_tuple(up_comming_sliperint, m));
        }
        delete current_sliperint;
    }

    return false;
}

/* Proper BFS
 */
[[ maybe_unused ]]
static
bool _solve3(Sliperint * const sliperint, Sliperint_displayer * const display) {
    std::vector<Sliperint*> done_with;
    std::vector<std::tuple<Sliperint*, move_t>> to_check;
    to_check.push_back(std::make_tuple(sliperint, END_MOVE));

    int check_counter = 0;

    Sliperint * front_sliperint;
    move_t direction;
    while (to_check.size()) {
        front_sliperint = std::get<0>(to_check.front());
        direction = std::get<1>(to_check.front());

        *(display->sliperint) += *front_sliperint;
        display->update(direction);

        if (is_in_goal_state(front_sliperint)) {
            display->success(check_counter);
            return true;
        }

        for (int m = 0; m < END_MOVE; m++) {
            Sliperint * appendix = new Sliperint(*front_sliperint);
            if (not is_operator(appendix, (move_t)m)) {
                continue;
            }
            apply_operator(appendix, (move_t)m);
            if (std::find_if(done_with.begin(),
                             done_with.end(),
                             [&appendix](const Sliperint* const i) {
                                 return *i == *appendix;
                             }) != done_with.end()
            ||  std::find_if(to_check.begin(),
                             to_check.end(),
                             [&appendix](const std::tuple<Sliperint*, move_t> &i) {
                                 return *std::get<0>(i) == *appendix;
                             }) != to_check.end()) {
                continue;
            }
            to_check.push_back(std::make_tuple(appendix, (move_t)m));
        }
        done_with.push_back(front_sliperint);
        to_check.erase(to_check.begin());
        ++check_counter;
    }

    return false;
}

/* Proper DFS
 */
[[ maybe_unused ]]
static
bool _solve4(Sliperint * const sliperint, Sliperint_displayer * const display) {
    std::vector<std::unique_ptr<Sliperint>> done_with;
    std::deque<std::tuple<Sliperint*, move_t>> to_check;
    to_check.push_back(std::make_tuple(sliperint, END_MOVE));

    int check_counter = 0;

    Sliperint * front_sliperint;
    move_t direction;
    while (to_check.size()) {
        front_sliperint = std::get<0>(to_check.front());
        direction = std::get<1>(to_check.front());
        to_check.pop_front();

        *(display->sliperint) += *front_sliperint;
        display->update(direction);

        if (is_in_goal_state(front_sliperint)) {
            display->success(check_counter);
            done_with.emplace_back(front_sliperint);
            return true;
        }

        for (int m = 0; m < END_MOVE; m++) {
            Sliperint * appendix = new Sliperint(*front_sliperint);
            if (not is_operator(appendix, (move_t)m)) {
                continue;
            }
            apply_operator(appendix, (move_t)m);
            if (std::find_if(done_with.begin(),
                             done_with.end(),
                             [&appendix](const std::unique_ptr<Sliperint> &i) {
                                 return *(i.get()) == *appendix;
                             }) != done_with.end()
            ||  std::find_if(to_check.begin(),
                             to_check.end(),
                             [&appendix](const std::tuple<Sliperint*, move_t> &i) {
                                 return *std::get<0>(i) == *appendix;
                             }) != to_check.end()) {
                continue;
            }
            to_check.push_front(std::make_tuple(appendix, (move_t)m));
        }
        done_with.emplace_back(front_sliperint);
        ++check_counter;
    }

    return false;
}

void solve(Sliperint * const sliperint, Sliperint_displayer * const display) {
    #if 0
    _solve(sliperint, display, END_MOVE, 0);
    _solve2(sliperint, display);
    _solve3(sliperint, display);
    #endif
    _solve4(sliperint, display);
}
