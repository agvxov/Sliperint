#include "tk_display.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tk.h>

static Tcl_Obj * tcl_player_position;

static int width, height;
static void * labyrinth;

typedef wall_t (*wall_getter_t)(void *, int, int);

static
const char * getWallHepler(Tcl_Interp *interp, const char **argv, wall_getter_t f) {
    static int x, y;
    sscanf(argv[1], "%d", &x);
    sscanf(argv[2], "%d", &y);
    return f(labyrinth, x, y) ? "true" : "false";
}

static
int Tcl_cGetVWall(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    const char * r = getWallHepler(interp, argv, get_vwall);
    Tcl_SetResult(interp, (char*)r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cGetHWall(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    const char * r = getWallHepler(interp, argv, get_hwall);
    Tcl_SetResult(interp, (char*)r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cGetPlayerX(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    char r[3];
    sprintf(r, "%d", get_player_x(labyrinth));
    Tcl_SetResult(interp, r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cGetPlayerY(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    char r[3];
    sprintf(r, "%d", get_player_y(labyrinth));
    Tcl_SetResult(interp, r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cGetGoalX(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    char r[3];
    sprintf(r, "%d", get_goal_x(labyrinth));
    Tcl_SetResult(interp, r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cGetGoalY(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    char r[3];
    sprintf(r, "%d", get_goal_y(labyrinth));
    Tcl_SetResult(interp, r, TCL_STATIC);
    return TCL_OK;
}

static
int Tcl_cUpdateDisplay(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    return TCL_OK;
}

static
void tcl_run(void) {
    Tcl_Interp * interp = Tcl_CreateInterp();
    if (interp == NULL) {
        fprintf(stderr, "Can't creae Tcl interpreter\n");
        exit(1);
    }
    Tk_Init(interp);

    Tcl_SetVar(interp, "WRAPPED", "true", 0); 

    char buf[3];
    sprintf(buf, "%d", width);
    Tcl_SetVar(interp, "width", buf, 0); 
    sprintf(buf, "%d", height);
    Tcl_SetVar(interp, "height", buf, 0); 

    tcl_player_position = Tcl_NewListObj(0, NULL);

    #define TCL_EASY_CREATE_COMMAND(c) do { \
        Tcl_CreateCommand(interp, #c, Tcl_ ## c, (ClientData)NULL, (void (*)())NULL); \
    } while (0)
    TCL_EASY_CREATE_COMMAND(cUpdateDisplay);
    TCL_EASY_CREATE_COMMAND(cGetHWall);
    TCL_EASY_CREATE_COMMAND(cGetVWall);
    TCL_EASY_CREATE_COMMAND(cGetPlayerX);
    TCL_EASY_CREATE_COMMAND(cGetPlayerY);
    TCL_EASY_CREATE_COMMAND(cGetGoalX);
    TCL_EASY_CREATE_COMMAND(cGetGoalY);

    int result = Tcl_EvalFile(interp, "gui.tcl");
    if (result == TCL_ERROR) {
        fprintf(stderr, "Tcl script execution failed: %s\n", Tcl_GetStringResult(interp));
        exit(1);
    }

    Tk_MainLoop();
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
