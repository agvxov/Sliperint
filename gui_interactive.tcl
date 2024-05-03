source gui.tcl

wm title . "Sliperint (interactive)"

button .back_button -text "Back" -command cBack
button .next_button -text "Next" -command cNext
pack .back_button
pack .next_button
