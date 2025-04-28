package main

import (
	"fmt"
	"github.com/gradgrind/minion_gui"
)

func main() {
	minion_gui.MinionGui(guidata, callback)
	//minion_gui.MinionGui("testA data", callback)
}

func callback(data string) string {
	fmt.Printf("Go callback got '%s'\n", data)
	return "GoCallbackResult: Value"
}

var guidata string = `# buttons
&APP_WIDTH: 600
&APP_HEIGHT: 400
&BACKGROUND: f0f0f0

GUI: [
  {
    NEW: Window
    NAME: MainWindow
    WIDTH: &APP_WIDTH
    HEIGHT: &APP_HEIGHT
    #ESC_CLOSES: 1
    DO: [
      [COLOUR &BACKGROUND]
    ]
  }
  {
    NEW: Vlayout
    NAME: l_MainWindow
    PARENT: MainWindow
    DO: [
      [fit_to_parent]
      [MARGIN 0]
    ]
  }
  # Top-level contents
  {
    NEW: PushButton
    NAME: PB1
    PARENT: l_MainWindow
    LABEL: "PushButton 1"
    DO: [
      # [FIXED 30]
    ]
  }
  {
    NEW: Output
    NAME: Output_1
    PARENT: l_MainWindow
    DO: [
      [clear_visible_focus]
      [LABEL "Button clicked:" 20] # adds padding box for label
      # [FIXED 30]
    ]
  }
  {
    NEW: PushButton
    NAME: PB2
    PARENT: l_MainWindow
    LABEL: "PushButton 2"
    DO: [
      # [FIXED 30]
    ]
  }
  
  {
    WIDGET: PB1
    DO: [
      [SIZE 200 60]
    ]
  }

  # Starting up the main window
  {
    WIDGET: MainWindow
    DO: [
      [SHOW]
    ]
  }
  {FUNCTION: RUN} # Enter main event loop
]
`
