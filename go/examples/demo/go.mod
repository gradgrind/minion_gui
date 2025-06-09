module demo

go 1.24

toolchain go1.24.1

replace github.com/gradgrind/minion_gui/go/mugui v0.0.0-20250609161849-d3ada3f2e13f => ../../mugui

require (
	github.com/gradgrind/minion/gominion v0.0.0-20250609043730-646470239d84
	github.com/gradgrind/minion_gui/go/mugui v0.0.0-20250609161849-d3ada3f2e13f
)
