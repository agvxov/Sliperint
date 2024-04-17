#ifndef SLIPERINT_SOLVER_HPP
#define SLIPERINT_SOLVER_HPP

#include "Sliperinth.hpp"

bool is_valid(Sliperinth * s);
bool is_operator(Sliperinth * s, move_t m);
bool is_in_goal_state(Sliperinth * s);
void apply_operator(Sliperinth * s, move_t m);
void solve(Sliperinth * sliperinth);

#endif
