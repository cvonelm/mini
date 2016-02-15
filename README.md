#mINI 0.2 - A **m**inimal **INI** parser
##Using the library

test/mini\_test.c contains a minimal example

##Documentation

###Structures

The last next pointer in both structures is set to NULL
####MINI\_Section

A section inside the INI File

- char \*name | the name of the section
- struct MINI\_Section \*next | the next section inside the INI File
- struct MINI\_KayValue \*values | a pointer to the first key-value pair of the section

####MINI\_KeyValue

A key-value pair.

- char \*key | the key
- char \*value | the value
- struct MINI\_KeyValue | the next key-value pair

###Functions

####struct MINI\_Section \*mini\_load(char \*text)

Loads an INI config from a string

**Arguments**
- char \*text | the INI File as a NULL-terminated string

**Returns**
the INI config on success, NULL if something went wrong
####struct MINI\_Section \*mini\_load\_file(char \*file)
Loads an INI config from a file

**Arguments**
- char \*text | the filename to be loaded

**Returns**
the INI config on success, NULL if something went wrong

#### void mini\_free(struct INI\_Section \*list)
Frees the INI\_Section list

**Arguments**
- struct INI\_Section \*list | the list which is to be freed

#### char mini\_query(struct INI\_Section \*list, char \*section, char \*key)
Queries for the value of a specific key inside a given section

**Arguments**
- struct INI\_Section \*list | the list which is scanned
- char \*section | the section in which the key-value pair is located
- char \*key | the key that is searched for
**Returns**
The value on success, NULL if section and/or key is not found

##Changelog
- 0.2 | added mini\_load\_file()
- 0.1 | inital release

##License
This project is open-source, for more information see the LICENSE
file

(c)2015-2016 Christian von Elm
