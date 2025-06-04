package minion

import (
	"fmt"
	"strconv"
)

const (
	T_NoType = iota
	T_String
	T_List
	T_Map
	T_Error
)

const (
	Token_End = iota
	Token_StartList
	Token_EndList
	Token_StartMap
	Token_EndMap
	Token_Comma
	Token_Colon
	Token_Macro
	Token_String
)

var token_text_map = map[int]string{
	Token_End:       "end of data",
	Token_StartList: "'['",
	Token_EndList:   "']'",
	Token_StartMap:  "'{'",
	Token_EndMap:    "'}'",
	Token_Comma:     "','",
	Token_Colon:     "':'",
}

type MValue interface {
	Size() int
}

type MString string

func (m MString) Size() int {
	return len(m)
}

type MList []MValue

func (m MList) Size() int {
	return len(m)
}

func (m MList) Get(index int) MValue {
	if index >= 0 && index <= len(m) {
		return m[index]
	}
	return nil
}

func (m MList) GetString(index int, s *string) bool {
	v := m.Get(index)
	if v != nil {
		snew, ok := v.(MString)
		if ok {
			*s = string(snew)
			return true
		}
		panic(fmt.Sprintf("List: expecting string at index: %d", index))
	}
	return false
}

func (m MList) GetInt(index int, i *int) bool {
	v := m.Get(index)
	if v != nil {
		snew, ok := m[index].(MString)
		if ok {
			i64, err := strconv.ParseInt(string(snew), 0, 0)
			if err != nil {
				panic(fmt.Sprintf(
					"List: invalid integer (%s) at index %d", snew, index))
			}
			*i = int(i64)
			return true
		}
		panic(fmt.Sprintf("List: expecting integer at index: %d", index))
	}
	return false
}

type MPair struct {
	Key   string
	Value MValue
}

type MMap []MPair

func (m MMap) Size() int {
	return len(m)
}

func (m MMap) Get(key string) MValue {
	for _, p := range m {
		if p.Key == key {
			return p.Value
		}
	}
	return nil
}

func (m MMap) GetString(key string, s *string) bool {
	v := m.Get(key)
	if v != nil {
		snew, ok := v.(MString)
		if ok {
			*s = string(snew)
			return true
		}
		panic(fmt.Sprintf("Map: expecting string value for key: %s", key))
	}
	return false
}

func (m MMap) GetInt(key string, i *int) bool {
	v := m.Get(key)
	if v != nil {
		snew, ok := v.(MString)
		if ok {
			i64, err := strconv.ParseInt(string(snew), 0, 0)
			if err != nil {
				panic(fmt.Sprintf(
					"Map: invalid integer value (%s) for key %s", snew, key))
			}
			*i = int(i64)
			return true
		}
		panic(fmt.Sprintf("Map: expecting integer value for key %s", key))
	}
	return false
}

// Used for recording read-position in input text
type position struct {
	line int
	char int
}

//type MinionMacros map[string]MValue

type InputBuffer struct {
	macro_map MMap

	// for character-by-character reading
	ch_input     string // input string
	ch_index     int
	ch_linestart int
	line_index   int

	// ch_buffer is used for constructinging strings before they are passed
	// to minion_value items.
	ch_buffer []byte //= make([]byte, 0, 100)
}

func (ib *InputBuffer) token_text(token int) string {
	if token == Token_String || token == Token_Macro {
		return "\"" + string(ib.ch_buffer) + "\""
	}
	return token_text_map[token]
}

func (ib *InputBuffer) get_macro(name string) MValue {
	m := ib.macro_map.Get(name)
	if m == nil {
		ib.error(fmt.Sprintf(
			"Unknown macro name: %s\n ... current position %s",
			name,
			pos(ib.here())))
	}
	return m
}

/* Read the next lexical "token" from the input.
 * If it is a string or a macro name, the actual string will be available
 * in `ch_buffer`.
 * If the input is invalid, the function "panics", passing a message.
 */
func (ib *InputBuffer) get_token() int {
	var ch byte
	for {
		ch = ib.read_ch(false)
		switch ch {
		// Act according to the next input character.
		case 0: // end of input, no next item
			return Token_End
		case ' ':
		case '\n': // continue seeking start of item
			continue
		case ':':
			return Token_Colon
		case ',':
			return Token_Comma
		case '[':
			return Token_StartList
		case ']':
			return Token_EndList
		case '{':
			return Token_StartMap
		case '}':
			return Token_EndMap
		case '"':
			ib.get_string()
			return Token_String
		case '&': // start of macro name
			ib.get_bare_string(ch)
			return Token_Macro
		case '#': // start comment
			ch = ib.read_ch(false)
			if ch == '[' {
				// Extended comment: read to "]#"
				comment_pos := ib.here()
				ch = ib.read_ch(false)
				for {
					if ch == ']' {
						ch = ib.read_ch(false)
						if ch == '#' {
							break
						}
						continue
					}
					if ch == 0 {
						ib.error(fmt.Sprintf("Unterminated comment ('\\[ ...') at position %s",
							pos(comment_pos)))
					}
					// Comment loop ... read next character
					ch = ib.read_ch(false)
				}
				// End of extended comment
			} else {
				// "Normal" comment: read to end of line
				for {
					if ch == '\n' || ch == 0 {
						break
					}
					ch = ib.read_ch(false)
				}
			}
			continue // continue seeking item
		default:
			ib.get_bare_string(ch)
			return Token_String
		}
	}
}

func (ib *InputBuffer) read_ch(instring bool) byte {
	if ib.ch_index >= len(ib.ch_input) {
		return 0
	}
	ch := ib.ch_input[ib.ch_index]
	ib.ch_index++
	if ch == '\n' {
		ib.line_index++
		ib.ch_linestart = ib.ch_index
		// these are not acceptable within delimited strings
		if !instring {
			// separator
			return ch
		}
		ib.error(fmt.Sprintf("Unexpected newline in delimited string, line %d",
			ib.line_index))
	} else if ch == '\r' || ch == '\t' {
		// these are acceptable in the source, but not within strings.
		if !instring {
			// separator
			return ' '
		}
	} else if ch >= 32 && ch != 127 {
		return ch
	}
	ib.error(fmt.Sprintf("Illegal character (0x%2x) at position %s",
		ch, pos(ib.here())))
	return 0 // unreachable
}

func (ib *InputBuffer) unread_ch() {
	if ib.ch_index == 0 {
		panic("[BUG] unread_ch reached start of data")
	}
	ib.ch_index--
	//NOTE: '\n' is never unread!
}

func (ib *InputBuffer) here() position {
	return position{ib.line_index + 1, ib.ch_index - ib.ch_linestart}
}

func pos(p position) string {
	return fmt.Sprintf("%d.%d", p.line, p.char)
}

/* Read a delimited string (terminated by '"') from the input.
 *
 * It is entered after the initial '"' has been read, so the next character
 * will be the first of the string.
 *
 * Escapes, introduced by '\', are possible. These are an extension of the
 * JSON escapes – see the MINION specification.
 *
 * The result is available in `ch_buffer`.
 */
func (ib *InputBuffer) get_string() {
	ib.ch_buffer = ib.ch_buffer[:0]
	start_pos := ib.here()
	var ch byte
	for {
		ch = ib.read_ch(true)
		if ch == '"' {
			break
		}
		if ch == 0 {
			ib.error(fmt.Sprintf(
				"End of data reached inside delimited string from position %s",
				pos(start_pos)))
		}
		if ch == '\\' {
			ch = ib.read_ch(false) // '\n' etc. are permitted here
			switch ch {
			case '"', '\\', '/':
				//break
			case 'n':
				ch = '\n'
				//break
			case 't':
				ch = '\t'
				//break
			case 'b':
				ch = '\b'
				//break
			case 'f':
				ch = '\f'
				//break
			case 'r':
				ch = '\r'
				//break
			case 'u':
				ib.add_unicode_to_ch_buffer(4)
				continue
			case 'U':
				ib.add_unicode_to_ch_buffer(6)
				continue
			case '[':
				// embedded comment, read to "\]"
				{
					comment_pos := ib.here()
					ch = ib.read_ch(false)
					for {
						if ch == '\\' {
							ch = ib.read_ch(false)
							if ch == ']' {
								break
							}
							continue
						}
						if ch == 0 {
							ib.error(fmt.Sprintf(
								"End of data reached inside string comment from position %s",
								pos(comment_pos)))
						}
						// loop with next character
						ch = ib.read_ch(false)
					}
				}
				continue // comment ended, seek next character
			default:
				ib.error(fmt.Sprintf(
					"Illegal string escape at position %s",
					pos(ib.here())))
			}
		}
		ib.ch_buffer = append(ib.ch_buffer, ch)
	}
}

// This version reads a non-delimited string.
// The result is available in `ch_buffer`.
func (ib *InputBuffer) get_bare_string(ch byte) {
	ib.ch_buffer = ib.ch_buffer[:0]
	for {
		ib.ch_buffer = append(ib.ch_buffer, ch)
		ch = ib.read_ch(true)
		switch ch {
		case ':', ',', ']', '}':
			ib.unread_ch()
			return
		case ' ', '\n', 0:
			return
		case '{', '[', '\\', '"':
			ib.error(fmt.Sprintf(
				"Unexpected character ('%c') at position %s",
				ch,
				pos(ib.here())))
		}
	}
}

func (ib *InputBuffer) error(msg string) {
	// Add most recently read characters
	ch_start := 0
	recent := ib.ch_index
	if recent > 80 {
		ch_start = ib.ch_index - 80
		// Find start of utf-8 sequence
		for {
			var ch byte = ib.ch_input[ch_start]
			if ch < 0x80 || (ch >= 0xC0 && ch < 0xF8) {
				break
			}
			ch_start++
		}
		recent = ib.ch_index - ch_start
	}
	panic(msg + "\n ... " + ib.ch_input[ch_start:recent])
}

func (ib *InputBuffer) get_list() MValue {
	var mlist MList
	for {
		t := ib.get_token()
		switch t {
		case Token_EndList:
			return mlist
		case Token_String:
			mlist = append(mlist, MString(ib.ch_buffer))
			//break
		case Token_StartList:
			mlist = append(mlist, ib.get_list())
			//break
		case Token_StartMap:
			mlist = append(mlist, ib.get_map())
			//break
		case Token_Macro:
			mlist = append(mlist, ib.get_macro(string(ib.ch_buffer)))
			//break
		default:
			ib.error(fmt.Sprintf(
				"Unexpected item whilst seeking list element: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
		}
		t = ib.get_token()
		if t == Token_Comma {
			continue
		}
		if t == Token_EndList {
			return mlist
		}
		ib.error(fmt.Sprintf(
			"Unexpected item whilst seeking comma in list: %s\n ... current position %s",
			ib.token_text(t), pos(ib.here())))
	}
}

func (ib *InputBuffer) get_map() MValue {
	var mmap MMap
	var key string
	for {
		t := ib.get_token()
		if t != Token_String {
			if t == Token_EndMap {
				return mmap
			}
			ib.error(fmt.Sprintf(
				"Unexpected item whilst seeking map element key: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
		}
		key = string(ib.ch_buffer)
		t = ib.get_token()
		if t != Token_Colon {
			ib.error(fmt.Sprintf(
				"Unexpected item whilst seeking map element colon: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
		}
		t = ib.get_token()
		switch t {
		case Token_String:
			mmap = append(mmap, MPair{key, MString(ib.ch_buffer)})
			//break
		case Token_StartList:
			mmap = append(mmap, MPair{key, ib.get_list()})
			//break
		case Token_StartMap:
			mmap = append(mmap, MPair{key, ib.get_map()})
			//break
		case Token_Macro:
			mmap = append(mmap, MPair{key, ib.get_macro(string(ib.ch_buffer))})
			//break
		default:
			ib.error(fmt.Sprintf(
				"Unexpected item whilst seeking map element value: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
		}
		t = ib.get_token()
		if t == Token_Comma {
			continue
		}
		if t == Token_EndMap {
			return mmap
		}
		ib.error(fmt.Sprintf(
			"Unexpected item whilst seeking comma in map: %s\n ... current position %s",
			ib.token_text(t), pos(ib.here())))
	}
}

// Convert a unicode code point (as hex string) to a UTF-8 string
func (ib *InputBuffer) add_unicode_to_ch_buffer(l int) {
	buf := make([]byte, l)
	for i := range l {
		buf[i] = ib.read_ch(false)
	}
	number, err := strconv.ParseUint(string(buf), 16, 0) // Parse as base 16
	if err == nil {
		s := string(rune(number))
		// Check validity (if invalid, the string will be "\uFFFD")
		if s != "\uFFFD" {
			// Add new character to read_buffer
			ib.ch_buffer = append(ib.ch_buffer, s...)
			return
		}
	}
	ib.error(fmt.Sprintf(
		"Invalid Unicode escape in string, position %s",
		pos(ib.here())))
}

func (ib *InputBuffer) Read(input_string string) (val MValue, error_message string) {
	// Prepare input buffer
	ib.ch_input = input_string
	ib.ch_index = 0
	ib.line_index = 0
	ib.ch_linestart = 0

	// Clear macros, just to be sure ...
	clear(ib.macro_map)

	// Catch errors (panic calls)
	defer func() {
		clear(ib.macro_map)
		if r := recover(); r != nil {
			e, ok := r.(string)
			if ok {
				error_message = e
			} else {
				panic(r)
			}
		}
	}()

	val = ib.read_val()
	return val, error_message
}

func (ib *InputBuffer) read_val() MValue {
	var m MValue
	var key string
	for {
		t := ib.get_token()
		switch t {
		case Token_String:
			m = MString(ib.ch_buffer)
			//break
		case Token_StartList:
			m = ib.get_list()
			//break
		case Token_StartMap:
			m = ib.get_map()
			//break
		case Token_Macro:
			key = string(ib.ch_buffer)
			t = ib.get_token()
			if t != Token_Colon {
				ib.error(fmt.Sprintf(
					"Unexpected item whilst seeking macro definition colon: %s\n ... current position %s",
					ib.token_text(t), pos(ib.here())))
			}
			t = ib.get_token()
			switch t {
			case Token_String:
				ib.macro_map = append(ib.macro_map, MPair{key, MString(ib.ch_buffer)})
				//break
			case Token_StartList:
				ib.macro_map = append(ib.macro_map, MPair{key, ib.get_list()})
				//break
			case Token_StartMap:
				ib.macro_map = append(ib.macro_map, MPair{key, ib.get_map()})
				//break
			case Token_Macro:
				ib.macro_map = append(ib.macro_map, MPair{key, ib.get_macro(string(ib.ch_buffer))})
				//break
			default:
				ib.error(fmt.Sprintf(
					"Unexpected item whilst seeking macro definition value: %s\n ... current position %s",
					ib.token_text(t), pos(ib.here())))
			}
			t = ib.get_token()
			if t == Token_Comma {
				continue
			}
			ib.error(fmt.Sprintf(
				"Expecting comma after macro definition, unexpected item: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
			//break
		default:
			ib.error(fmt.Sprintf(
				"Unexpected item whilst seeking top-level element: %s\n ... current position %s",
				ib.token_text(t), pos(ib.here())))
		}
		t = ib.get_token()
		if t == Token_End {
			break
		}
		ib.error(fmt.Sprintf(
			"Expecting end of data, unexpected item: %s\n ... current position %s",
			ib.token_text(t), pos(ib.here())))
	}
	return m
}

type DumpBuffer struct {
	indent int // = 2;
	depth  int
	buffer []byte
}

// *******************************

// *** dump functions (serializing)

func (db *DumpBuffer) dump_string(source string) {
	db.buffer = append(db.buffer, '"')
	var ch byte
	for _, ch = range []byte(source) {
		if ch >= 32 {
			if ch == 127 {
				db.buffer = append(db.buffer, '\\', 'u', '0', '0', '7', 'F')
				continue
			}
			db.buffer = append(db.buffer, ch)
			continue
		}
		switch ch {
		case '"':
			db.buffer = append(db.buffer, '\\', '"')
			//break
		case '\n':
			db.buffer = append(db.buffer, '\\', 'n')
			//break
		case '\t':
			db.buffer = append(db.buffer, '\\', 't')
			//break
		case '\b':
			db.buffer = append(db.buffer, '\\', 'b')
			//break
		case '\f':
			db.buffer = append(db.buffer, '\\', 'f')
			//break
		case '\r':
			db.buffer = append(db.buffer, '\\', 'r')
			//break
		case '\\':
			db.buffer = append(db.buffer, '\\', '\\')
			//break
		default:
			db.buffer = append(db.buffer, '\\', 'u', '0', '0')
			if ch >= 16 {
				db.buffer = append(db.buffer, '1')
				ch -= 16
			} else {
				db.buffer = append(db.buffer, '0')
			}
			if ch >= 10 {
				db.buffer = append(db.buffer, 'A'+ch-10)
			} else {
				db.buffer = append(db.buffer, '0'+ch)
			}
		}
	}
	db.buffer = append(db.buffer, '"')
}

func (db *DumpBuffer) dump_pad(n int) {
	if n >= 0 {
		db.buffer = append(db.buffer, '\n')
		n *= indentation
		for n > 0 {
			db.buffer = append(db.buffer, ' ')
			n--
		}
	}
}

func dump_list(source MinionValue, indent int) bool {
	db.buffer = append(db.buffer, '[')
	a := source.Data.(MinionArray)
	if len(a) != 0 {
		var new_depth int = -1
		if indent >= 0 {
			new_depth = indent + 1
		}
		for _, m := range a {
			dump_pad(new_depth)
			if !dump_value(m, new_depth) {
				return false
			}
			db.buffer = append(db.buffer, ',')
		}
		db.buffer = db.buffer[:len(db.buffer)-1] // remove last ','
		dump_pad(indent)
	}
	db.buffer = append(db.buffer, ']')
	return true
}

func dump_map(source MinionValue, indent int) bool {
	db.buffer = append(db.buffer, '{')
	a := source.Data.(MinionPairArray)
	if len(a) != 0 {
		var new_depth int = -1
		if indent >= 0 {
			new_depth = indent + 1
		}
		for _, m := range a {
			dump_pad(new_depth)
			dump_string(string(m.Key))
			db.buffer = append(db.buffer, ':')
			if indent >= 0 {
				db.buffer = append(db.buffer, ' ')
			}
			if !dump_value(m.Value, new_depth) {
				return false
			}
			db.buffer = append(db.buffer, ',')
		}
		db.buffer = db.buffer[:len(db.buffer)-1] // remove last ','
		dump_pad(indent)
	}
	db.buffer = append(db.buffer, '}')
	return true
}

func dump_value(source MinionValue, indent int) bool {
	var ok bool = true
	switch source.Type {
	case T_String:
		// Strings don't receive any extra formatting
		s := source.Data.(MinionString)
		dump_string(string(s))
		//break
	case T_Array:
		ok = dump_list(source, indent)
		//break
	case T_PairArray:
		ok = dump_map(source, indent)
		//break
	default:
		ok = false
	}
	return ok
}

func MinionDump(source MinionValue, indent int) (string, bool) {
	db.buffer = db.buffer[:0] // reset db.buffer
	if dump_value(source, indent) {
		return string(db.buffer), true
	}
	return "", false
}
