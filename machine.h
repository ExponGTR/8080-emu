#ifndef MACHINE_H
#define MACHINE_H

#include "emu.h"
#define KEY_COIN 'c'
#define KEY_P1_LEFT 'a'
#define KEY_P1_RIGHT 's'
#define KEY_P1_FIRE ' '
#define KEY_P1_START '1'
#define KEY_PAUSE 'p'

typedef struct Machine
{
    State *state;
    uint8_t lsb;
    uint8_t msb;
    uint8_t shift_offset;
} Machine;

#endif // MACHINE_H