#mINI - A **m**inimal **INI** parser

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

#### int mini\_load(struct INI\_Section \*\*list, char \*text)

Loads an INI file

- struct INI\_Section \*\*list | the head of the section-list will be laid into the pointer pointed by this pointer
- char \*file | the INI File as a NULL-terminated string
- Returns: 0 on success, -1 if something failed

#### void mini\_free(struct INI\_Section \*list)
Frees the INI\_Section list

- struct INI\_Section \*list | the list which is to be freed

#### char mini\_query(struct INI\_Section \*list, char \*section, char \*key)
Queries for a the value of a specific key inside a given section

- struct INI\_Section \*list | the list which is scanned
- char \*section | the section in which the key-value pair is located
- char \*key | the key that is searched for
- Returns: the value on success, NULL if section and/or key is not found

##TODO:

- Real error handling in lemux\_ini\_load, it currently just informs you that there is an error 
  not what is wrong, neither where it happened
##License
This project is open-source, for more information see the LICENSE
file

(c)2015-2016 Christian von Elm
