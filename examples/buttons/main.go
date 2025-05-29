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
      [TEXT, "PushButton 1"],
      # [FIXED 30]
    ]
  },
  {
    NEW: Output,
    NAME: Output_1,
    DO: [
      [LABEL "Pushed:" 20]
      [clear_visible_focus],
      [HEIGHT, 50],
      [WIDTH, 400]
    ]
  },
  {
    NEW: PushButton,
    NAME: PB2,
    DO: [
      TEXT: "PushButton 2",
      # [FIXED 30]
    ]
  },
  {
    NEW: Vgrid,
    NAME: l_MainWindow,
    PARENT: MainWindow,
    ITEMS: [
      [PB1 HORIZONTAL FIXED],
      [Output_1 CENTRE FIXED],
      [PB2 CENTRE FIXED]
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
