/* (c) 2015-2016 Christian von Elm */
/* this project is open source, see the LICENSE file for more information */
#include "mini.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
char *skip_wspace(char *text);
char *get_name(char **text);
char *handle_comment(char *text);
char *get_value(char **text);

void mini_write(FILE* stream, struct MINI_Section *list)
{
    for(;list != NULL; list = list->next)
    {
        fprintf(stream, "[%s]\n", list->name);
        struct MINI_KeyValue *kv;
        for(kv = list->values;kv != NULL; kv = kv->next)
        {
            fprintf(stream, "%s=%s\n", kv->key, kv->value);
        }
    }
}
struct MINI_KeyValue *mini_get_section(struct MINI_Section *ini, char *name)
{
    struct MINI_Section *it = ini;
    for(;it != NULL;it = it->next)
    {
        if(strcmp(it->name, name) == 0)
        {
            return it->values;
        }
    }
    
}
struct MINI_Section *mini_load_file(char *file)
{
    size_t size;
    FILE *f = fopen(file, "rb");
    if(f == NULL)
    {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *str;
    str = calloc(1, size + 1);
    if(str == NULL)
    {
        return NULL;
    }
    fread(str, size, 1, f);
    if(ferror(f))
    {
        free(str);
        fclose(f);
        return NULL;
    }
    struct MINI_Section *ini = mini_load(str);
    free(str);
    fclose(f);
    return ini;

}
struct MINI_Section *mini_load(char *text)
{
    char *it = text;
    
    struct MINI_Section *section_it = NULL;
    struct MINI_Section *section_start = NULL;
    struct MINI_KeyValue *value_it = NULL;
    
    while(*it != 0)
    {
        if(*it == '#')
        {
            it = handle_comment(it);
            if(*it == 0)
            {
                return section_start;
            }
        }
        else if(*it == '[')
        {
            it++;
            it = skip_wspace(it);
            if(*it == 0)
            {
                return NULL;
            }

            if(section_start == NULL)
            {
                section_start = calloc(1, sizeof(struct MINI_Section));
                if(section_start == NULL)
                {
                    return NULL;
                }
                section_it = section_start;
                section_it->next = NULL;
            }
            else
            {
                section_it->next = calloc(1, sizeof(struct MINI_Section));
                if(section_it->next == NULL)
                {
                    return NULL;
                }
                section_it = section_it->next;
                section_it->next = NULL;
            }
            section_it->values = NULL;
            section_it->name = get_name(&it);
            if(*section_it->name == 0)
            {
                return NULL;
            }
            it = skip_wspace(it);
            if(*it == 0)
            {
                return NULL;
            }
            if(*it != ']')
            {
                return NULL;
            }
            it++;

            while(1)
            {
                it = skip_wspace(it);
                if(*it == 0)
                {
                    return section_start;
                }
                else if(*it == '#')
                {
                    it = handle_comment(it);
                    if(*it == 0)
                    {
                        return section_start;
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
                            return NULL;
                        }
                        value_it = section_it->values;
                        value_it->next = NULL;
                    }
                    else
                    {
                        value_it->next = calloc(1, sizeof(struct MINI_KeyValue));
                        if(value_it->next == NULL)
                        {
                            return NULL;
                        }
                        value_it = value_it->next;
                        value_it->next = NULL;
                    }

                    value_it->key = get_name(&it);
                    if(*value_it->key == 0)
                    {
                        return NULL;
                    }

                    it = skip_wspace(it);
                    if(*it == 0)
                    {
                        return NULL;
                    }
                    if(*it != '=')
                    {
                        return NULL;
                    }

                    /* first letter after = */
                    it++;
                    if(*it == 0)
                    {
                        return NULL;
                    }

                    it = skip_wspace(it);
                    if(*it == 0)
                    {
                        return NULL;
                    }

                    value_it->value = get_value(&it);
                    if(*value_it->value == 0)
                    {
                        return NULL;
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
    
    for(kv_it = it->values;kv_it != NULL && strcmp(kv_it->key, key) != 0;kv_it = kv_it->next);
    
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
            if(kv->key != NULL)
            {
                free(kv->key);
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
char *get_value(char  **text)
{
    char *name_end_ptr = *text;
    for(;*name_end_ptr != '\n' &&  *name_end_ptr != 0;name_end_ptr++);
    char * result  = calloc(name_end_ptr - *text + 1, sizeof(char));
    if(result == NULL)
    {
        return NULL;
    }
    memcpy(result, *text, name_end_ptr - *text);
    *text = name_end_ptr;
    return result;
}
char *handle_comment(char *text)
{
    for(;*text != '\n' && *text != 0;text++);
    if(*text == '\n')
    {
        text++;
    }
    return text;
}
char *get_name(char **text)
{
    char *name_end_ptr = *text;

    for(;(isalnum(*name_end_ptr) || *name_end_ptr == '_')
            && *name_end_ptr != 0 ;name_end_ptr++);
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

char *skip_wspace(char *text)
{
    for(;isspace(*text) && *text != 0;++text);
    return text;
}
