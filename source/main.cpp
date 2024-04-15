#include "Sliperinth.hpp"
#include "Sliperinth_displayer_tk.hpp"


signed main() {
    player_t player((coord_t){0, 0});

    Sliperinth map;
    map.set_walls(0, {true, true, true, true});
    map.set_walls(1, {true, false, true, false});
    map.set_walls(2, {true, false, false, false});
    map.set_walls(3, {false, false, true, false});

    Sliperinth_displayer_tk display(&map);
    display.update();

    while (true) { ; }
}
