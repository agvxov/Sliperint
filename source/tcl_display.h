#ifndef TCL_DISPLAYER_H
#define TCL_DISPLAYER_H
#include <tcl.h>
#include <stdbool.h>

extern Tcl_Interp * interp;

extern void tcl_setup(int w, int h, void * lab);

extern void tcl_loop(void);

extern void tcl_set_border(int x, int y, bool v);

#endif
