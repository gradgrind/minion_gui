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
&OUTPUT_COLOUR: ffc8ff

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
    PARENT: l_MainWindow
    FILL: HORIZONTAL
    GROW: 0
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
    PARENT: CoursePanel
    FILL: HORIZONTAL
    GROW: 0
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
    PARENT: ViewSelect
    FILL: CENTRE
    GROW: 0
    DO: [
      [clear_visible_focus]
      [WIDTH 100]
      [ADD "Type 1" "Type 2"]
    ]
  }
  {
    NEW: Choice
    NAME: TableRow
    PARENT: ViewSelect
    FILL: CENTRE
    GROW: 0
    DO: [
      [LABEL "X X X X X X:" 10] # adds padding box for label
      [clear_visible_focus]
      [WIDTH 150]
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
    FILL: HORIZONTAL
    DO: [
      # [COLOUR &OUTPUT_COLOUR]
      [VALUE "Read only ˝Öößŋħĸ€"]
      [clear_visible_focus]
      [LABEL "Total lessons:" 20] # adds padding box for label
    ]
  }
  {
    NEW: Hgrid
    NAME: ViewSelect
    PARENT: CoursePanel   
    FILL: HORIZONTAL
    GROW: 0
    DO: [
      [MARGIN 5]
      [GAP 10]
      [HEIGHT 60]
    ]
  }
  # --- ViewSelect: end Top Panel

  # +++ Content: horizontal layout, table and editor form
  {
    NEW: RowTable
    NAME: T1
    PARENT: Content
    FILL: FILL
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
  
  # +++ EditorForm: EditForm layout
  {
    NEW: PopupEditor
    NAME: EF1
    PARENT: EditorForm
    LABEL: "EDITOR 1"
  }  
  {
    NEW: TextEditor
    NAME: EF2
    PARENT: EditorForm
    LABEL: "TEXT 1"
  }  
  {
    NEW: TextEditor
    NAME: EF2X
    PARENT: EditorForm
    LABEL: "TEXT 2"
  }  
  {
    NEW: Choice
    NAME: EF3
    PARENT: EditorForm
    LABEL: "CHOICE 1"
  }
  {
    NEW: Separator
    NAME: Sep1
    PARENT: EditorForm
  }
  {
    NEW: PopupEditor
    NAME: EF4
    PARENT: EditorForm
    LABEL: "ANOTHER EDITOR"
  }  
  {
    NEW: Checkbox
    NAME: EF5
    PARENT: EditorForm
    LABEL: "CHECKBOX with a long label"
  }  
  {
    NEW: List
    NAME: EF6
    PARENT: EditorForm
    LABEL: "List entry"
  }  
  {
    NEW: EditForm
    NAME: EditorForm
    PARENT: Content
    FILL: VERTICAL
    GROW: 0
    DO: [
      [WIDTH 300]
    ]
  }
  # --- EditorForm
  
  {
    NEW: Hgrid
    NAME: Content
    PARENT: CoursePanel
    FILL: FILL
    DO: [
      [GAP 3]
    ]
  }
  # --- Content
  {
    NEW: Vgrid
    NAME: CoursePanel
    PARENT: l_MainWindow
    FILL: FILL
  }
  # --- CoursePanel
  
  {
    NEW: Vgrid
    NAME: l_MainWindow
    PARENT: MainWindow
    DO: [
      [fit_to_parent]
      # [MARGIN 5]
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
