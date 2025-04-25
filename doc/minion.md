# MINION: MINImal Object Notation, v.4

MINION is a simple data-transfer format taking some basic ideas
from JSON. It has features which make it suitable for easily readable
and writable configuration-files.
 
The only basic data type is the string. In addition there are containers:
lists (arrays) and maps (objects / associative arrays). Files must be encoded
as UTF-8. Most of the ASCII control characters (0-31 and 127) are not allowed
and should be reported as errors. The permitted exceptions are '\n',
'\t', '\r' as layout/spacing characters, but not within strings.
There are other Unicode characters which should probably be avoided,
but no checks are made as this is generally a difficult problem and it
is perhaps not clear where the line should be drawn.

The parsed data (not the source text!) is completely compatible with
parsed JSON, so a conversion to JSON should be fairly straightforward.

## Details

A string may be enclosed in quotation marks (" ... "), but this is not
necessary if no "special" characters are included in the string.

Whitespace characters are necessary as separators between items only when the
separation is not clear otherwise. They may, however, be added freely.

A plain comment starts with a '#' character and continues to the end of the
line. However, if the '#' is directly followed by '[', the comment is
only terminated by "]#" and can continue over line breaks.

The "special" characters are:
 - "whitespace" characters (space, newline, etc.) – separators
 - '#': start a comment
 - ':': separates key from value in a map
 - '{': start a map
 - '}': end a map
 - '[': start a list
 - ']': end a list
 - '"': string delimiter
 - '\': string "escape" character (allowed in delimited string)

### Maps

A map (like a JSON object) associates a value with a key:
```
    { key1:value1 key2:value2 }
```
 - A "key" is a string.
 - A "value" may be a string, a list or a map.
 
### Lists

A list (like a JSON array) is an ordered collection of values:
```
[ value1 value2 ]
```
 - A "value" may be a string, a list or a map.

### Strings

Certain characters are not directly possible in a string, they may be
included (only when the string is delimited by '"' characters) by means
of escape sequences:
 - '"': "\'"
 - '\': "\/"
 - tab: "\t"
 - newline: "\n"
 - hexadecimal Unicode character: `\{xxxx}` or `\{xxxxx}`

In addition, it is possible to have an "embedded comment" in a
delimited string. This starts with "\[" and is ended by "]\".
As it may include newlines, this may be used to split a string
over several lines.

### The "top level"

The top level of a MINION text is a map – without the surrounding
braces ({ ... }).
 
### "Macro" feature

There is a very limited macro-like feature. Elements declared at the
top level which start with '&' may be referenced (which basically means
included) at any later point in a data structure by means of the macro
name, e.g.:
```
&MACRO1: [A list of words]

    ...

DEF1: { X: &MACRO1 }
```
Macro keys are not included in the resulting data map.
 
Note that keys beginning with '&' at lower levels will neither themselves
be replaced nor used to define replacement values.
