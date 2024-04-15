package require Tk

if {![info exists ::WRAPPED]} {
    set ::width 10
    set ::height 10
    proc cUpdateDisplay {} {}
    proc cGetWall {x, y} { return "true" }
}

set tileWidth 50

wm title . "Ice Labyrinth"
frame .board
pack .board
for {set x 0} {$x < $::width} {incr x} {
    for {set y 0} {$y < $::height} {incr y} {
        canvas .board.c$x$y -width $::tileWidth -height $tileWidth
        if {[cGetWall $x $y]} {
            .board.c$x$y create line 0 0 0 $tileWidth -fill black -width 10
            grid .board.c$x$y -column $x -row $y 
        }
    }
}

proc updateDisplay {} {
    cUpdateDisplay
    after 100 updateDisplay
}

after 100 updateDisplay
bind . <Destroy> {exit}
