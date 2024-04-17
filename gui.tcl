package require Tk

if {![info exists ::WRAPPED]} {
    set ::width 10
    set ::height 10
    proc cUpdateDisplay {} {}
    proc cGetWall {x, y} { return "true" }
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

    set limit [expr $::height - 1]
    set limit2 [expr $::width - 1]

    for {set i 0} {$i < $limit} {incr i} {
        for {set h 0} {$h < $limit2} {incr h} {
            new_tile $h $i $::tileWidth $::tileWidth
        }
    }

    for {set i 0} {$i < $limit} {incr i} {
        for {set h 0} {$h < $limit2} {incr h} {
            if {[cGetVWall $h $i]} {
                vertical_wall .board.c$h$i
            }
            if {[cGetHWall $h $i]} {
                horizontal_wall .board.c$h$i
            }
        }
    }


    for {set i 0} {$i < [expr $limit + 1]} {incr i} {
        if {[cGetVWall $limit2 $i]} {
            new_tile $limit $i $::wallWidth $::tileWidth
            vertical_wall .board.c$limit$i
        }
    }

    for {set i 0} {$i < [expr $limit2 + 1]} {incr i} {
        if {[cGetHWall $i $limit]} {
            new_tile $i $limit $::tileWidth $::wallWidth
            horizontal_wall .board.c$i$limit
        }
    }
}

proc draw_circle {x y color} {
    set circleRadious [expr $::tileWidth - 10]
    .board.c$x$y create oval $circleRadious $circleRadious 10 10 -fill $color 
}

proc displayPlayer {} {
    set ::playerPosition [list [cGetPlayerX] [cGetPlayerY]]

    #draw_circle {*}$::playerOldPosition white
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
