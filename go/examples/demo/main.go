package main

import (
	"fmt"
	"path/filepath"
	"strings"

	"github.com/gradgrind/minion/gominion"
	"github.com/gradgrind/minion_gui/go/mugui"
)

func main() {
	fp, err := filepath.Abs(
		filepath.Join("..", "..", "..", "examples", "demo.minion"))
	if err != nil {
		panic(err)
	}
	fpm := fmt.Sprintf(
		`[[MINION_FILE,"%s"],[WIDGET,MainWindow,[SHOW]],[RUN]]`,
		fp)
	mugui.MinionGui(fpm, callback)
}

var tabs = map[string]bool{}

// TODO
func callback(data string) string {
	fmt.Printf("Go callback got '%s'\n", data)
	v := gominion.ReadMinion(data)
	if e, ok := v.(gominion.MError); ok {
		fmt.Println(" *** Error ***")
		fmt.Println(e)
	} else {
		fmt.Println("  -->")
		fmt.Println(gominion.DumpMinion(v, -1))
	}
	mm := v.(gominion.MList)
	var cbr string
	var wname string
	mm.GetString(0, &wname)

	if wname == "()buttons" {

		cbr = "[[WIDGET_PREFIX, buttons]"
		wtab := "(buttons)l_MainWindow"
		if !tabs[wtab] {
			cbr += ",[MINION_FILE, ../../../examples/buttons.minion]"
			cbr += ",[WIDGET, ()viewer, [ADD, "
			cbr += wtab
			cbr += "]]"
			tabs[wtab] = true
		}
		cbr += ",[WIDGET, ()viewer, [SELECT, "
		cbr += wtab
		cbr += "]]]"

	} else if wname == "()grid" {

		cbr = "[[WIDGET_PREFIX, grid]"
		wtab := "(grid)l_MainWindow"
		if !tabs[wtab] {
			cbr += ",[MINION_FILE, ../../../examples/grid.minion]"
			cbr += ",[WIDGET, ()viewer, [ADD, "
			cbr += wtab
			cbr += "]]"
			tabs[wtab] = true
		}
		cbr += ",[WIDGET, ()viewer, [SELECT, "
		cbr += wtab
		cbr += "]]]"

	} else if wname == "()complex" {

		cbr = "[[WIDGET_PREFIX, complex]"
		wtab := "(complex)l_MainWindow"
		if !tabs[wtab] {
			cbr += ",[MINION_FILE, ../../../examples/complex.minion]"
			cbr += ",[WIDGET, ()viewer, [ADD, "
			cbr += wtab
			cbr += "]]"
			tabs[wtab] = true
		}
		cbr += ",[WIDGET, ()viewer, [SELECT, "
		cbr += wtab
		cbr += "]]]"

	} else if strings.HasPrefix(wname, "(buttons)") {

		cbr = "[[WIDGET, Output_1, [VALUE, "
		cbr += wname
		cbr += "]]]"

	} else if strings.HasPrefix(wname, "(grid)") {

		cbr = "[[WIDGET, "
		cbr += wname
		cbr += ", [TEXT, \""
		cbr += wname
		cbr += " pushed\"]]]"

	} else if strings.HasPrefix(wname, "(complex)") {

		cbr = "[[WIDGET, TableTotals, [VALUE, "
		cbr += gominion.DumpString(data)
		cbr += "]]"

		if wname == "(complex)EF1" || wname == "(complex)EF4" {
			cbr += ", [WIDGET, popup, [SHOW, "
			cbr += wname
			cbr += "]]"
		}
		cbr += "]"

	} else {
		panic("Unknown Callback" + gominion.DumpString(data))
	}

	fmt.Println("CB: " + cbr)
	return cbr
}
