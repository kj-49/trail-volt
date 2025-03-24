#include "mode.h"

static mode_e mode;

mode_e mode_get()
{
    return mode;
}

void mode_set(mode_e new_mode)
{
    mode = new_mode;
}
