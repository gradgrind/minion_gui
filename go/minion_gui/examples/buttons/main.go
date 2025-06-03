package main

import (
	"fmt"
	"github.com/gradgrind/minion_gui/go/minion_gui"
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

{GUI: [
  {
    NEW: Window,
    NAME: MainWindow,
    FLOATING: 1,
    WIDTH: &APP_WIDTH,
    HEIGHT: &APP_HEIGHT,
    #ESC_CLOSES: 1,
    DO: [
      [TEXT, "Buttons Test"],
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
  {
    NEW: Label,
    NAME: Label_1,
    PROPERTIES: {
      LABEL_ALIGN: LEFT
    },
    DO: [
      [TEXT, "Pushed"]
    ]
  },
  {
    NEW: Output,
    NAME: Output_1,
    PROPERTIES: {
      GRID_ALIGN: HORIZONTAL,
      GRID_GROW: 1
    },
    DO: [
      [clear_visible_focus]
    ]
  },
  {
    NEW: PushButton,
    NAME: PB2,
    DO: [
      [TEXT, "PushButton 2"]
    ]
  },
  {
    NEW: Hlayout,
    NAME: l_output,
    PROPERTIES: {
      GRID_ALIGN: HORIZONTAL
    },
    DO: [
      # [HEIGHT, 80],
    ],
    WIDGETS: [
      Label_1, Output_1
    ]
  },
  {
    NEW: Vlayout,
    NAME: l_MainWindow,
    PARENT: MainWindow,
    WIDGETS: [
      PB1, l_output, PB2
    ],
    DO: [
      [fit_to_parent],
      [MARGIN, 5],
      [GAP, 10]
      # [COLOUR, ff0000] # No effect (FL_NO_BOX)
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
}
`
