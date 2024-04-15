package require Tk

if {![info exists ::WRAPPED]} {
    set ::width 10
    set ::height 10
    proc cUpdateDisplay {} {}
    proc cGetWall {x, y} { return "true" }
}

wm title . "Ice Labyrinth"
frame .board
pack .board
for {set x 0} {$x < $::width} {incr x} {
    for {set y 0} {$y < $::height} {incr y} {
        canvas .board.c$x$y -width 50 -height 50
        if {[cGetWall $x $y]} {
            puts [cGetWall $x $y]
            set a "-width 10"
        }
        .board.c$x$y create line 0 0 0 50 -fill black {*}$a
        grid .board.c$x$y -column $x -row $y 
    }
}

proc updateDisplay {} {
    cUpdateDisplay
    after 100 updateDisplay
}

after 100 updateDisplay
bind . <Destroy> {exit}
