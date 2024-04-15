#include "tcl_display.h"
#include "atom.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tk.h>

Tcl_Interp * interp;

static int width, height;
static void * labyrinth;

extern wall_t get_wall(void * lab, int x, int y);

static
int Tcl_cGetWall(ClientData clientData, Tcl_Interp *interp, int argc, char **argv) {
    static int x, y;
    sscanf(argv[1], "%d", &x);
    sscanf(argv[2], "%d", &y);
    const char * r = get_wall(labyrinth, x, y) ? "true" : "false";
    Tcl_SetResult(interp, (char*)r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cUpdateDisplay(ClientData clientData, Tcl_Interp *interp, int argc, char **argv) {
    return TCL_OK;
}

static
void tcl_run(void) {
    interp = Tcl_CreateInterp();
    if (interp == NULL) {
        fprintf(stderr, "Can't creae Tcl interpreter\n");
        exit(1);
    }
    Tk_Init(interp);

    Tcl_SetVar(interp, "WRAPPED", "true", 0); 

    Tcl_SetVar(interp, "msg", "msg", 0); 
    char buf[3];
    sprintf(buf, "%d", width);
    Tcl_SetVar(interp, "width", buf, 0); 
    sprintf(buf, "%d", height);
    Tcl_SetVar(interp, "height", buf, 0); 

    Tcl_CreateCommand(interp, "cUpdateDisplay", Tcl_cUpdateDisplay, (ClientData)NULL, (void (*)())NULL);
    Tcl_CreateCommand(interp, "cGetWall", Tcl_cGetWall, (ClientData)NULL, (void (*)())NULL);

    int result = Tcl_EvalFile(interp, "gui.tcl");
    if (result == TCL_ERROR) {
        fprintf(stderr, "Tcl script execution failed: %s\n", Tcl_GetStringResult(interp));
        exit(1);
    }

    Tk_MainLoop();
}

void tcl_set_border(int x, int y, bool v) {
    static char cmd[32];
    sprintf(cmd, "setBorder %d %d %d", x, y, v);
    Tcl_Eval(interp, cmd);
}

void tcl_setup(int w, int h, void * lab) {
    width = w;
    height = h;
    labyrinth = lab;
}

void tcl_loop(void) {
    pthread_t tcl_thread;
    pthread_create(&tcl_thread, NULL, (void *(*)(void*))tcl_run, (void*)NULL);
}
