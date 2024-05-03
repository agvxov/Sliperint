#include "tk_display.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tcl.h>
#include <tk.h>

static Tcl_Obj * tcl_player_position;

static int width, height;
static void * labyrinth;

typedef wall_t (*wall_getter_t)(void *, int, int);

static
int Tcl_cGetState(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    const char * r = (char*)get_state(labyrinth);
    Tcl_SetResult(interp, (char*)r, TCL_STATIC);
    return TCL_OK;
}

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
int Tcl_cBack(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    display_back(labyrinth);
    return TCL_OK;
}

static
int Tcl_cNext(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv) {
    display_next(labyrinth);
    return TCL_OK;
}

static inline 
void tcl_run_init(Tcl_Interp * interp) {
    Tcl_Init(interp);
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
    TCL_EASY_CREATE_COMMAND(cGetState);
}

#define CHECK_INTERPRETER(interp) do { \
    if (interp == NULL) { \
        fprintf(stderr, "Can't creae Tcl interpreter\n"); \
        exit(1); \
    } \
} while (0);

#define CHECK_SCRIPT_LOAD(interp, file) do { \
    int result = Tcl_EvalFile(interp, file); \
    if (result == TCL_ERROR) { \
        fprintf(stderr, "Tcl script execution failed: %s\n", Tcl_GetStringResult(interp)); \
        exit(1); \
    } \
} while (0);

static
void tcl_run(void) {
    Tcl_Interp * interp = Tcl_CreateInterp();
    CHECK_INTERPRETER(interp);

    tcl_run_init(interp);

    CHECK_SCRIPT_LOAD(interp, "gui.tcl");

    Tk_MainLoop();
}

static
void tcl_run_interactive(void) {
    Tcl_Interp * interp = Tcl_CreateInterp();
    CHECK_INTERPRETER(interp);

    tcl_run_init(interp);

    TCL_EASY_CREATE_COMMAND(cBack);
    TCL_EASY_CREATE_COMMAND(cNext);

    CHECK_SCRIPT_LOAD(interp, "gui_interactive.tcl");

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

void tcl_loop_interactive(void) {
    pthread_t tcl_thread;
    pthread_create(&tcl_thread, NULL, (void *(*)(void*))tcl_run_interactive, (void*)NULL);
}
