#include "../mini.h"
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    struct MINI_Section *ini = mini_load("[foo]\n"
            "bar = baz\n"
            "#blabla\n");
    if(ini == NULL)
    {
        return -1;
    }
    FILE *f = fopen("test.ini", "w");
    mini_write(f, ini);
    fclose(f);
    mini_free(ini);
}
