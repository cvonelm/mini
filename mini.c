/* (c) 2015-2016 Christian von Elm */
/* this project is open source, see the LICENSE file for more information */
#include "mini.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
void mini_print(struct MINI_Section *list)
{
    for(;list != NULL; list = list->next)
    {
        printf("[%s]\n", list->name);
        struct MINI_KeyValue *kv;
        for(kv = list->values;kv != NULL; kv = kv->next)
        {
            printf("%s=%s\n", kv->name, kv->value);
        }
    }
}
int mini_load(struct MINI_Section **list, char *text)
{
    char *end_ptr = text + strlen(text);
    char *it = text;
    struct MINI_Section *section_it;
    struct MINI_KeyValue *value_it = NULL;
    while(it < end_ptr)
    {
        if(*it == '#')
        {
            it = handle_comment(it, end_ptr);
            if(it == NULL)
            {
                return 0;
            }
        }
        else if(*it == '[')
        {
            it++;
            it = skip_wspace(it, end_ptr);
            if(it == NULL)
            {
                return -1;
            }

            if(*list == NULL)
            {
                *list = calloc(1, sizeof(struct MINI_Section));
                if(*list == NULL)
                {
                    return -1;
                }
                section_it = *list;
                section_it->next = NULL;
            }
            else
            {
                section_it->next = calloc(1, sizeof(struct MINI_Section));
                if(section_it->next == NULL)
                {
                    return -1;
                }
                section_it = section_it->next;
                section_it->next = NULL;
            }
            section_it->values = NULL;
            section_it->name = get_name(&it, end_ptr);
            if(section_it->name == NULL)
            {
                return -1;
            }
            it = skip_wspace(it, end_ptr);
            if(it == NULL)
            {
                return -1;
            }
            if(*it != ']')
            {
                return -1;
            }
            it++;

            while(1)
            {
                it = skip_wspace(it, end_ptr);
                if(it == NULL)
                {
                    return 0;
                }
                else if(*it == '#')
                {
                    it = handle_comment(it, end_ptr);
                    if(it == NULL)
                    {
                        return 0;
                    }
                }
                else if(*it == '[')
                {
                    break;
                }
                else
                {
                    if(section_it->values == NULL)
                    {
                        section_it->values = calloc(1, sizeof(struct MINI_KeyValue));
                        if(section_it->values == NULL)
                        {
                            fprintf(stderr, "Bla\n");
                            return -1;
                        }
                        value_it = section_it->values;
                        value_it->next = NULL;
                    }
                    else
                    {
                        value_it->next = calloc(1, sizeof(struct MINI_KeyValue));
                        if(value_it->next == NULL)
                        {
                            return -1;
                        }
                        value_it = value_it->next;
                        value_it->next = NULL;
                    }
                    value_it->name = get_name(&it, end_ptr);
                    if(value_it->name == NULL)
                    {
                        fprintf(stderr, "bla\n");
                        return -1;
                    }
                    fprintf(stderr, "(%c)\n", *it);
                    it = skip_wspace(it, end_ptr);
                    if(it == NULL)
                    {
                        return -1;
                    }
                    if(*it != '=')
                    {
                        return -1;
                    }
                    /* first letter after = */
                    it++;
                    if(it >= end_ptr)
                    {
                        return -1;
                    }
                    it = skip_wspace(it, end_ptr);
                    if(it == NULL)
                    {
                        return -1;
                    }
                    value_it->value = get_value(&it, end_ptr);
                    if(value_it->value == NULL)
                    {
                        return -1;
                    }
                }
            }
        }
        if(*it == '[')
        {
            continue;
        }
        it++;
    }
    return 0;
}
char *mini_query(struct MINI_Section *list, char *section, char *key)
{
    struct MINI_Section *it;
    /* stop if it = null or name = section */
    for(it = list;it != NULL && strcmp(it->name, section) != 0;it = it->next);
    
    if(it == NULL)
    {
        return NULL;
    }
    
    struct MINI_KeyValue *kv_it;
    
    for(kv_it = it->values;kv_it != NULL && strcmp(kv_it->name, key) != 0;kv_it = kv_it->next);
    
    if(kv_it == NULL)
    {
        return NULL;
    }
    char *result = malloc(strlen(kv_it->value) + 1);
    if(result == NULL)
    {
        return NULL;
    }
    memcpy(result, kv_it->value, strlen(kv_it->value));
    result[strlen(kv_it->value)] = 0;
    return result;
}
void mini_free(struct MINI_Section *list)

{
    while(list != NULL)
    {
        struct MINI_Section *next = list->next;
        if(list->name != NULL)
        {
            free(list->name);
        }
        struct MINI_KeyValue *kv = list->values;
        while(kv != NULL)
        {
            struct MINI_KeyValue *kv_next = kv->next;
            if(kv->name != NULL)
            {
                free(kv->name);
            }
            if(kv->value != NULL)
            {
                free(kv->value);
            }
            free(kv);
            kv = kv_next;
        }
        free(list);
        list = next;
    }
}
char *get_value(char  **text, char *end_ptr)
{
    char *name_end_ptr = *text;
    for(;*name_end_ptr != '\n' &&  name_end_ptr < end_ptr;name_end_ptr++);
    char * result  = calloc(name_end_ptr - *text + 1, sizeof(char));
    if(result == NULL)
    {
        return NULL;
    }
    memcpy(result, *text, name_end_ptr - *text);
    *text = name_end_ptr;
    return result;
}
char *handle_comment(char *text, char *end_ptr)
{
    for(;*text != '\n' && text < end_ptr;text++);
    text++;
    if(text == end_ptr)
    {
        return NULL;
    }
    return text;
}
char *get_name(char **text, char *end_ptr)
{
    char *name_end_ptr = *text;

    for(;(isalnum(*name_end_ptr) || *name_end_ptr == '_')&& name_end_ptr != end_ptr;name_end_ptr++);
    char *result = calloc(name_end_ptr- (*text) + 1, sizeof(char));
    if(result == NULL)
    {
        return NULL;
    }
    result[name_end_ptr - (*text)] = 0;
    memcpy(result, *text, name_end_ptr-(*text));
    
    *text = name_end_ptr;
    return result;
}

char *skip_wspace(char *text, char *end_ptr)
{
    for(;isspace(*text) && text < end_ptr;text++);
    if(text < end_ptr)
    {
        return text;
    }
    else
    {
        return NULL;
    }
}
