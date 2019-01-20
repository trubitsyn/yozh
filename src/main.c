#include <stdio.h>

#include "yozh.h"

int main()
{
    if (!yozh_init())
    {
        printf("Failed to initialize yozh. Terminating.\n");
        return 1;
    }
    yozh_loop();
    return yozh_terminate();
}