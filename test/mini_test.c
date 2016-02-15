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
    mini_print(ini);
    mini_free(ini);
}
