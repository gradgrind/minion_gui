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
&OUTPUT_COLOUR: ffc8ff # ffffc8 ?

GUI: [
  # *** The main window
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
  
  # +++ l_MainWindow: top-level layout
  {
    NEW: Box
    NAME: P1
    #PARENT: l_MainWindow
    DO: [
      [LABEL "Panel 1"]
      [BOXTYPE FL_BORDER_FRAME]
      [HEIGHT 100]
    ]
  }
  
  # +++ CoursePanel: vertical layout
  
  # * Top Panel
  {
    NEW: Box
    NAME: todo
    #PARENT: CoursePanel
    DO: [
      [LABEL "Another Panel"]
      [BOXTYPE FL_FLAT_BOX]
      [HEIGHT 50]
    ]
  }
  
  # +++ ViewSelect: horizontal layout
  {
    NEW: Choice
    NAME: TableType
    DO: [
      [clear_visible_focus]
      [WIDTH 100]
      [ADD "Type 1" "Type 2"]
    ]
  }
  {
    NEW: Label
    NAME: label1
    LABEL: "X X X X X X:"
    ALIGN: RIGHT
    DO: [
      [LABEL "X X X X X X:"]
      # [BOXTYPE FL_FLAT_BOX]
      # [HEIGHT 50]
      [WIDTH 100]
    ]
  }
  {
    NEW: Choice
    NAME: TableRow
    DO: [
      # [LABEL "X X X X X X:" 10] # adds padding box for label
      [clear_visible_focus]
      [WIDTH 150]
      [ADD "FB: Fritz Jolander Jeremias Braun"
           "DG: Diego Garcia"
           "PM: Pamela Masterson"
      ]
    ]
  }
  {
    NEW: Label
    NAME: label2
    LABEL: "Total lessons:"
    ALIGN: RIGHT
    DO: [
      # [LABEL "Total lessons:"]
      # [BOXTYPE FL_FLAT_BOX]
      # [HEIGHT 50]
      [WIDTH 110]
    ]
  }
  {
    NEW: Output
    NAME: TableTotals
    DO: [
      # [COLOUR &OUTPUT_COLOUR]
      [VALUE "Read only ˝Öößŋħĸ€"]
      [clear_visible_focus]
      # [LABEL "Total lessons:" 20] # adds padding box for label
    ]
  }
  {
    NEW: Row
    NAME: ViewSelect
    #PARENT: CoursePanel   
    ITEMS: [
      [TableType CENTRE FIXED]
      [label1 CENTRE FIXED]
      [TableRow CENTRE FIXED]
      [label2 CENTRE FIXED]
      [TableTotals HORIZONTAL]
    ]
    
    DO: [
      [MARGIN 5]
      # [GAP 10]
      [HEIGHT 60]
    ]
  }
  # --- ViewSelect: end Top Panel

  # +++ Content: horizontal layout, table and editor form
  {
    NEW: RowTable
    NAME: T1
    #PARENT: Content
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
    #PARENT: Content
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
      [WIDTH 300]
    ]
  }
  {
    NEW: Row
    NAME: Content
    #PARENT: CoursePanel
    DO: [
      [GAP 3]
    ]
    ITEMS: [
      [T1 FILL]
      [EditorForm VERTICAL FIXED]
    ]
  }
  # --- Content
  {
    NEW: Column
    NAME: CoursePanel
    #PARENT: l_MainWindow
    ITEMS: [
      [todo HORIZONTAL FIXED]
      [ViewSelect HORIZONTAL FIXED]
      [Content FILL]
    ]
  }
  # --- CoursePanel
  
  {
    NEW: Column
    NAME: l_MainWindow
    PARENT: MainWindow
    DO: [
      [fit_to_parent]
      # [MARGIN 5]
    ]
    ITEMS: [
      [P1 HORIZONTAL FIXED]
      [CoursePanel FILL]
    ]
  }
  # --- l_MainWindow
  
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
