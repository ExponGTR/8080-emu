#ifndef EMU_H
#define EMU_H

#include <stdint.h>
typedef struct Condition_Codes
{
    uint8_t z:1; // Z (zero) flag
    uint8_t s:1; // S (sign) flag
    uint8_t p:1; // P (parity) flag
    uint8_t cy:1; // CY (carry) flag
    uint8_t ac:1; // AC (auxillary carry) flag
    uint8_t pa:3; // Padding for the byte
} Condition_Codes;

typedef struct State
{
    uint8_t a; // Register A
    uint8_t b; // Register B
    uint8_t c; // Register C
    uint8_t d; // Register D
    uint8_t e; // Register E
    uint8_t h; // Register H
    uint8_t l; // Register L
    uint16_t sp; // Stack pointer
    uint16_t pc; // Program counter
    uint8_t *memory; // Main memory (16K)
    Condition_Codes flags; // Flags
    uint8_t int_enable; // Interrupt enable
    uint8_t halted; // Halted CPU state
} State;

#endif // EMU_H