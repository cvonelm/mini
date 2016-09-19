/* (c) 2015-2016 Christian von Elm
 * this project is open source, see the LICENSE file for more information 
 */
#ifndef _MINI_H_
#define _MINI_H_
#include <stdio.h>

struct MINI_Section
{
    char *name;
    struct MINI_Section *next;
    struct MINI_KeyValue *values;
};
struct MINI_KeyValue
{
    char *name;
    char *value;
    struct MINI_KeyValue *next;
};

struct MINI_Section *mini_load(char * text);
struct MINI_Section *mini_load_file(char *file);

void mini_write(FILE *stream, struct MINI_Section *list);

void mini_free(struct MINI_Section *list);

char *mini_query(struct MINI_Section *list, char *section, char *key);

#endif
