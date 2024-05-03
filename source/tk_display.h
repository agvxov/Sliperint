#ifndef TCL_DISPLAYER_H
#define TCL_DISPLAYER_H
#include <tcl.h>
#include <stdbool.h>

extern void tcl_setup(int w, int h, void * lab);

extern void tcl_loop(void);

extern void tcl_loop_interactive(void);

extern void tcl_set_border(int x, int y, bool v);

// XXX
#include "atom.h"
extern const char * get_state(void * lab);
extern wall_t get_vwall(void * lab, int x, int y);
extern wall_t get_hwall(void * lab, int x, int y);
extern int get_player_x(void * lab);
extern int get_player_y(void * lab);
extern int get_goal_x(void * lab);
extern int get_goal_y(void * lab);
extern void display_back(void * d);
extern void display_next(void * d);

#endif
