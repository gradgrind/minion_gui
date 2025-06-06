module buttons

go 1.24

toolchain go1.24.1

replace github.com/gradgrind/minion_gui/go/mugui v0.0.0-00010101000000-000000000000 => ../../mugui

require (
	github.com/gradgrind/minion/gominion v0.0.0-20250606050747-424406a9e7ef
	github.com/gradgrind/minion_gui/go/mugui v0.0.0-00010101000000-000000000000
)
