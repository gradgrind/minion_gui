package minion

import (
	"fmt"
	"os"
	"testing"
	"time"
)

func TestM1(t *testing.T) {
	fmt.Println("\n +++ TestM1:\n ==============")
	ib := InputBuffer{}
	doc, e := ib.Read("{A: 1 :}")
	if len(e) == 0 {
		fmt.Println("  -->")
		dump := Dumper()
		fmt.Println(dump(doc, -1))
	} else {
		fmt.Println(" *** Error ***")
		fmt.Println(e)
	}
}

func TestM2(t *testing.T) {
	fmt.Println("\n +++ TestM2:\n ==============")
	content, err := os.ReadFile("../data/test4.minion")
	if err != nil {
		fmt.Println("$ File Error: " + err.Error())
		return
	}
	input := string(content)
	ib := InputBuffer{}

	for range 10 {
		t0 := time.Now()
		ib.Read(input)
		t1 := time.Now()
		fmt.Println(t1.Sub(t0))
	}
	fmt.Println("  ---------------------------------- ")

	doc, e := ib.Read(input)

	if len(e) == 0 {
		fmt.Println("  -->")
		dump := Dumper()
		fmt.Println(dump(doc, 0))
	} else {
		fmt.Println(" *** Error ***")
		fmt.Println(e)
	}
}
