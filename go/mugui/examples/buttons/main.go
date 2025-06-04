package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

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

	doc, e := MinionRead(data)
	if len(e) == 0 {
		fmt.Println("  -->")
		fmt.Println(MinionDump(doc.Item, -1))
	} else {
		fmt.Println(" *** Error ***")
		fmt.Println(e)
	}

	return "GoCallbackResult:\"" + strings.ReplaceAll(data, "\"", "\\'") + "\""
}

/*
   auto m = minion_ibuffer.read(data);
   auto mm = m.m_map();
   string wname;
   (*mm)->get_string("CALLBACK", wname);
   printf("callback got '%s'\n", dump(m));
   minion::MMap mp({{"WIDGET", "Output_1"}, {"DO", {{"VALUE", wname}}}});
   auto cbr = dump(mp);
   //printf("??? %s\n", cbr);
   //fflush(stdout);
   return cbr;

*/
