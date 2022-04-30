#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void check_error(int res, char *message)
{
    if (res != 0) {
        perror(message);
        exit(EXIT_FAILURE);
    }
}