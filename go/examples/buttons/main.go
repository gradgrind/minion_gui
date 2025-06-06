package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/gradgrind/minion/gominion"
	"github.com/gradgrind/minion_gui/go/mugui"
)

func main() {
	fp := filepath.Join("..", "..", "..", "examples", "buttons1.minion")
	content, err := os.ReadFile(fp)
	if err != nil {
		fmt.Println(err)
	} else {
		guidata := string(content)
		mugui.MinionGui(guidata, callback)
	}
}

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
	mm := v.(gominion.MMap)
	var wname string
	mm.GetString("CALLBACK", &wname)

	mp := gominion.MMap{
		gominion.MPair{Key: "WIDGET", Value: gominion.MString("Output_1")},
		gominion.MPair{Key: "DO", Value: gominion.MList{gominion.MList{gominion.MString("VALUE"), gominion.MString(wname)}}}}
	cbr := gominion.DumpMinion(mp, -1)
	fmt.Println("CB: " + cbr)
	return cbr
}
