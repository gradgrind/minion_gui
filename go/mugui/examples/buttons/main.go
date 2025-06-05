package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/gradgrind/minion_gui/go/mugui"
)

func main() {
	fp := filepath.Join("..", "..", "..", "..", "examples", "buttons1.minion")
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

	ib := InputBuffer{}
	dump := Dumper()
	doc, e := ib.Read(data)
	if len(e) == 0 {
		fmt.Println("  -->")
		fmt.Println(dump(doc, -1))
	} else {
		fmt.Println(" *** Error ***")
		fmt.Println(e)
	}

	mm := doc.(MMap)
	var wname string
	mm.GetString("CALLBACK", &wname)

	mp := MMap{
		{"WIDGET", MString("Output_1")},
		//TODO: segfault ...
		//{"DO", MList{MMap{{"VALUE", MString(wname)}}}}}
		{"DO", MList{MList{MString("VALUE"), MString(wname)}}}}
	cbr := dump(mp, -1)
	fmt.Println("CB: " + cbr)
	return cbr
}
