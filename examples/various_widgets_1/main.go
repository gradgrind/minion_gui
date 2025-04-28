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

var guidata string = `# Various widgets
&APP_WIDTH: 1000
&APP_HEIGHT: 700
&BACKGROUND: f0f0f0
&OUTPUT_COLOUR: ffffc8

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
      # [MARGIN 5]
    ]
  }
  # Top-level contents
  {
    NEW: Box
    NAME: P1
    PARENT: l_MainWindow
    DO: [
      [LABEL "Panel 1"]
      [BOXTYPE FL_BORDER_FRAME]
      [FIXED 100]
    ]
  }
  # *** Complex Panel ***
  {
    NEW: Vlayout
    NAME: ComplexPanel
    PARENT: l_MainWindow
  }
  # Top Panel
  {
    NEW: Box
    NAME: todo
    PARENT: ComplexPanel
    DO: [
      [LABEL "Another Panel"]
      [BOXTYPE FL_FLAT_BOX]
      [FIXED 50]
    ]
  }
  {
    NEW: Hlayout
    NAME: ViewSelect
    PARENT: ComplexPanel
    DO: [
      [MARGIN 5]
      [GAP 5]
      [FIXED 60]
    ]
  }
  {
    NEW: Choice
    NAME: TableType
    PARENT: ViewSelect
    DO: [
      [clear_visible_focus]
      [FIXED 100]
      [ADD "Type 1" "Type 2"]
    ]
  }
  {
    NEW: Choice
    NAME: TableRow
    PARENT: ViewSelect
    DO: [
      [LABEL "X X X X X X:" 10] # adds padding box for label
      [clear_visible_focus]
      [FIXED 150]
      [ADD "FB: Fritz Jolander Jeremias Braun"
           "DG: Diego Garcia"
           "PM: Pamela Masterson"
      ]
    ]
  }
  {
    NEW: Output
    NAME: TableTotals
    PARENT: ViewSelect
    DO: [
      [COLOUR &OUTPUT_COLOUR]
      [VALUE "Read only ˝Öößŋħĸ€"]
      [clear_visible_focus]
      [LABEL "Total lessons:" 20] # adds padding box for label
    ]
  }

  # Top Panel – end

  # Left/Right: table and editor form
  {
    NEW: Hlayout
    NAME: Content
    PARENT: ComplexPanel
    DO: [
      [GAP 3]
    ]
  }
  
  {
    NEW: RowTable
    NAME: T1
    PARENT: Content
    DO: [
      # [COLOUR ffffe0]
      [col_headers "Col 1" "Column 2" "Col 3" "Col 4"]
      [col_header_height 30]
      # [row_headers "Row 1" "Long row 2" "Row 3"]
      [row_header_width 80]
      [row_height_all 30]
      # [col_header_color ffe8e8]
      [row_header_color ffe8e8]
      [add_row "Row 1" A B C D]
      [add_row "Long Row 2" a b c d]
      [add_row "Row 3" "" "" "A very long table entry" last]
    ]
  }
  
  {
    NEW: EditForm
    NAME: EditorForm
    PARENT: Content
    ITEMS: [
      [EDITOR EF1 "EDITOR 1"]
      [TEXT EF2 "TEXT 1"]
      [TEXT EF2X "TEXT2"]
      [CHOICE EF3 "CHOICE 1"]
      [SEPARATOR]
      [EDITOR EF4 "ANOTHER EDITOR"]
      [CHECKBOX EF5 "CHECKBOX with a long label"]
      [LIST EF6 "List entry"]
    ]
    DO: [
      [FIXED 300]
    ]
  }
  
  {
    WIDGET: EF1
    DO: [
      [VALUE "Changed First"]
    ]
  }
  {
    WIDGET: EF3
    DO: [
      [ADD Item1 Item2]
    ]
  }
  {
    WIDGET: EF6
    DO: [
      [SET "New list 1" "New list 2"]
    ]
  }
  
  # Starting up the main window
  {
    WIDGET: MainWindow
    DO: [
      # [RESIZABLE l_MainWindow]
      [SHOW]
    ]
  }
  {FUNCTION: RUN} # Enter main event loop
]
`
