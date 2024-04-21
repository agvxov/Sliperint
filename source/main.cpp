#include "Sliperint.hpp"
#include "Sliperint_displayer_tk.hpp"
#include "solver.hpp"

signed main() {
    Sliperint * map = (
        #include "offical.inc"
    );

    Sliperint_displayer_tk display(map);
    //Sliperint_displayer_basic display(map);

    solve(map, &display);

    return 0;
}
