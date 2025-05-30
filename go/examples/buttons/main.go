package main

import (
	"fmt"
	"github.com/gradgrind/minion_gui"
	"strings"
)

func main() {
	minion_gui.MinionGui(guidata, callback)
	//minion_gui.MinionGui("testA data", callback)
}

func callback(data string) string {
	fmt.Printf("Go callback got '%s'\n", data)
	return "GoCallbackResult:\"" + strings.ReplaceAll(data, "\"", "\\'") + "\""
}

var guidata string = `# buttons
&APP_WIDTH: 600,
&APP_HEIGHT: 400,
&BACKGROUND: f0f0f0,

GUI: [
  {
    NEW: Window,
    NAME: MainWindow,
    WIDTH: &APP_WIDTH,
    HEIGHT: &APP_HEIGHT,
    #ESC_CLOSES: 1,
    DO: [
      [COLOUR, &BACKGROUND]
    ]
  },
  # Top-level contents
  {
    NEW: PushButton,
    NAME: PB1,
    DO: [
      [TEXT, "PushButton 1"]
    ]
  },
  TODO: Need a sepaate label:
  {
    NEW: Label,
    NAME: Label_1,
    DO: [
      [TEXT, "Pushed"]
    ]
  },
  {
    NEW: Output,
    NAME: Output_1,
    DO: [
      [clear_visible_focus]
    ]
  },
  {
    NEW: PushButton,
    NAME: PB2,
    DO: [
      TEXT: "PushButton 2"
    ]
  },
  {
    NEW: Vgrid,
    NAME: l_MainWindow,
    PARENT: MainWindow,
    WIDGETS: [
      [PB1, CENTRE, 0],
      [Label_1 RIGHT, 0],
      [Output_1, CENTRE, 0],
      [PB2, CENTRE, 0],
    ],
    DO: [
      [fit_to_parent],
      [MARGIN, 0]
    ]
  },
  
  {
    WIDGET: PB1,
    DO: [
      [SIZE, 200, 60]
    ]
  },

  # Starting up the main window
  {
    WIDGET: MainWindow,
    DO: [
      [SHOW]
    ]
  },
  {FUNCTION: RUN} # Enter main event loop
]
`
