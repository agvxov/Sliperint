package require Tk

if {![info exists ::WRAPPED]} {
    set ::width 10
    set ::height 10
    proc cUpdateDisplay {} {}
    proc cGetHWall {x, y} { return "true" }
    proc cGetVWall {x, y} { return "true" }
}

set tileWidth 50
set wallWidth 10
set playerOldPosition {0 0}
set playerPosition {0 0}

wm title . "Sliperinth"
frame .board
pack .board


proc drawWalls {} {
    proc new_tile {x y width height} {
        canvas .board.c$x$y -width $width -height $height
        grid .board.c$x$y -column $x -row $y 
    }

    proc horizontal_wall {canvas} {
        $canvas create line 0 0 $::tileWidth 0 -fill black -width $::wallWidth
    }

    proc vertical_wall {canvas} {
        $canvas create line 0 0 0 $::tileWidth -fill black -width $::wallWidth
    }

    set height_limit [expr $::height - 1]
    set width_limit [expr $::width - 1]

    # create and fill the full sized tiles
    for {set i 0} {$i < $::width} {incr i} {
        for {set h 0} {$h < $::height} {incr h} {
            new_tile $i $h $::tileWidth $::tileWidth
            if {[cGetVWall $i $h]} {
                vertical_wall .board.c$i$h
            }
            if {[cGetHWall $i $h]} {
                horizontal_wall .board.c$i$h
            }
        }
    }

    for {set i 0} {$i < $::height} {incr i} {
        new_tile $::width $i $::wallWidth $::tileWidth
        vertical_wall .board.c$::width$i
    }

    for {set i 0} {$i < $::width} {incr i} {
        new_tile $i $::height $::tileWidth $::wallWidth
        horizontal_wall .board.c$i$::height
    }
}

proc draw_circle {x y color} {
    set circleRadious [expr $::tileWidth - 10]
    .board.c$x$y create oval $circleRadious $circleRadious 10 10 -fill $color 
}

proc displayPlayer {} {
    set ::playerPosition [list [cGetPlayerX] [cGetPlayerY]]

    draw_circle {*}$::playerOldPosition white
    draw_circle {*}$::playerPosition blue

    set ::playerOldPosition $::playerPosition
}

proc displayGoal {} {
    draw_circle [cGetGoalX] [cGetGoalY] red
}

proc updateDisplay {} {
    cUpdateDisplay
    displayPlayer
    after 100 updateDisplay
}

drawWalls
displayGoal

after 100 updateDisplay
bind . <Destroy> {exit}
