#include "logic.h"
#include <stdio.h>

void process(char *input_buffer, char *output_buffer)
{
    sprintf(output_buffer, "This is your echo '%s'", input_buffer);
}
