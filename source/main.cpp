#include "Sliperint.hpp"
#include "Sliperint_displayer_tk.hpp"
#include "Sliperint_displayer_tk_interactive.hpp"
#include "solver.hpp"

signed main() {
    Sliperint * map = (
        #include "offical.inc"
    );

    #if 0
    Sliperint_displayer_basic display(map);
    Sliperint_displayer_tk display(map);
    #endif
    Sliperint_displayer_tk_interactive display(map);

    solve(map, &display);

    while (true) { ; }

    return 0;
}
