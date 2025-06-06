module grid

go 1.24

toolchain go1.24.1

replace github.com/gradgrind/minion_gui/go/mugui v0.0.0-20250606093235-816e896918eb => ../../mugui

require (
	github.com/gradgrind/minion/gominion v0.0.0-20250606090724-dc0696277c62
	github.com/gradgrind/minion_gui/go/mugui v0.0.0-20250606093235-816e896918eb
)
