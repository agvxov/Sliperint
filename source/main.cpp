#include "Sliperinth.hpp"
#include "Sliperinth_displayer_tk.hpp"
#include "sliperint_solver.hpp"

signed main() {
    Sliperinth * map = (
        #include "test_map.inc"
    );

    Sliperinth_displayer_tk display(map);
    display.update();

    solve(map);

    return 0;
}
