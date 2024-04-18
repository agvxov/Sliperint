#ifndef SLIPERINT_SOLVER_HPP
#define SLIPERINT_SOLVER_HPP

#include "Sliperint.hpp"
#include "Sliperint_displayer.hpp"

bool is_valid(Sliperint * s);
bool is_operator(Sliperint * s, move_t m);
bool is_in_goal_state(Sliperint * s);
void apply_operator(Sliperint * s, move_t m);
void solve(Sliperint * const sliperint, Sliperint_displayer * display);

#endif
