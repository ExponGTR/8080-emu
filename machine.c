#include "emu.c"
#include "machine.h"
uint8_t machine_IN(Machine *machine, uint8_t port)
{
    uint8_t input;
    switch (port)
    {
        case 3:
            uint16_t val = (machine->msb << 8) | machine->lsb;
            input = (val >> (8 - machine->shift_offset)) & 0xff;
            break;
    }
    return input;
}
int main(void)
{
    State *state = init_8080();
    Machine *machine = malloc(sizeof(Machine));
    machine->state = state;
    machine->lsb = 0;
    machine->msb = 0;
    machine->shift_offset = 0;

    read_file_to_mem(state, "invaders.h", 0);
    read_file_to_mem(state, "invaders.g", 0x800);
    read_file_to_mem(state, "invaders.f", 0x1000);
    read_file_to_mem(state, "invaders.e", 0x1800);
    while (!state->halted)
    {
        uint8_t opcode = fetch(state);
        if (opcode == 0xdb) // IN
        {
            uint8_t port = fetch(state);
            state->a = machine_IN(machine, port);
        }
        emulate_8080(state, opcode);
    }
    return 0;
}