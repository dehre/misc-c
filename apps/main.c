#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modern.h"

static char random_text[] = "The scarlet dragonfly is a species of dragonfly in the family "
                            "Libellulidae";

int main(void)
{
    printf("%s\n", random_text);
    char *token = strtok(random_text, " ");
    while (token)
    {
        printf("%s ", reverse_in_place(token));
        token = strtok(NULL, " ");
    }
    return 0;
}
