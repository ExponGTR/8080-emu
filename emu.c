/*
The disassembler is still not working completely how it should, but we can worry about that later
Note:
    M is the memory location pointed to by the register pair HL
    AC flag will not be implemented for now (maybe after I get Space Invaders running)
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Condition_Codes
{
    uint8_t z:1;
    uint8_t s:1;
    uint8_t p:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t pa:3;
} Condition_Codes;

typedef struct State
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;
    uint16_t pc;
    uint8_t *memory;
    Condition_Codes flags;
    uint8_t int_enable;     
} State;

void unimplemented_instruction(State *state)
{
    printf("Error: Unimplemented instruction\n");
    exit(1);
}

uint8_t fetch(State *state)
{
    uint8_t byte = state->memory[state->pc];
    state->pc++;
    return byte;
}

int parity(int val); // Write this later

void ADD(State *state, uint8_t reg)
{
    uint16_t ans = (uint16_t) state->a + (uint16_t) reg; // Converting to 16 bit for accurate flag setting ig
    state->flags.z = ((ans & 0xff) == 0);
    state->flags.s = ((ans & 0x80) == 0x80);
    state->flags.cy = (ans > 0xff);
    state->flags.p = parity(ans & 0xff);
    state->a = ans & 0xff;
    state->pc += 1;
}
void ADC(State *state, uint8_t reg)
{
    uint16_t ans = (uint16_t) state->a + (uint16_t) reg + (uint16_t) state->flags.cy;
    state->flags.z = ((ans & 0xff) == 0);
    state->flags.s = ((ans & 0x80) == 0x80); // S is set when the 7th bit is set
    state->flags.cy = (ans > 0xff);
    state->flags.p = parity(ans & 0xff);
    state->a = ans & 0xff;
    state->pc += 1;
}
void SUB(State *state, uint8_t reg)
{
    uint16_t ans = (uint16_t) state->a - (uint16_t) reg;
    state->flags.z = ((ans & 0xff) == 0);
    state->flags.s = ((ans & 0x80) == 0x80);
    state->flags.cy = (ans > 0xff);
    state->flags.p = parity(ans & 0xff);
    state->a = ans & 0xff;
    state->pc += 1;
}
void SBB(State *state, uint8_t reg)
{
    uint16_t ans = (uint16_t) state->a - (uint16_t) reg - (uint16_t) state->flags.cy;
    state->flags.z = ((ans & 0xff) == 0);
    state->flags.s = ((ans & 0x80) == 0x80);
    state->flags.cy = (ans > 0xff);
    state->flags.p = parity(ans & 0xff);
    state->a = ans & 0xff;
    state->pc += 1;
}
void INX(State *state, uint8_t *high_reg, uint8_t *low_reg)
{
    (*low_reg)++;
    if (*low_reg == 0)
    {
        (*high_reg)++;
    }
    state->pc += 1;
}
void INR(State *state, uint8_t *reg)
{
    uint16_t ans = (uint16_t) *reg + 1;
    state->flags.z = ((ans & 0xff) == 0);
    state->flags.s = ((ans & 0x80) == 0x80);
    state->flags.p = parity(ans & 0xff);
    *reg = ans & 0xff;
    state->pc += 1;
}
void DCX(State *state, uint8_t *high_reg, uint8_t *low_reg)
{
    (*low_reg)--;
    if (*low_reg == 0xff)
    {
        (*high_reg)--;
    }
    state->pc += 1;
}
void DCR(State *state, uint8_t *reg)
{
    uint16_t ans = (uint16_t) *reg - 1;
    state->flags.z = (ans == 0);
    state->flags.s = ((ans & 0x80) == 0x80);
    state->flags.p = parity(ans & 0xff);
    *reg = ans & 0xff;
    state->pc += 1;
}
void DAD(State *state, uint8_t high_reg, uint8_t low_reg)
{
    uint32_t hl = (state->h << 8) | (state->l);
    uint32_t ans = hl + (uint32_t) (((high_reg) << 8) | (low_reg));
    state->flags.cy = (ans > 0xffff);
    state->h = (ans & 0xff00) >> 8; // Just upper byte to H
    state->l = (ans & 0x00ff); // And lower byte to L
    state->pc += 1;
}
void JMP(State *state, unsigned char *opcode)
{
    state->pc = (opcode[2] << 8) | (opcode[1]); 
}
void CALL(State *state, unsigned char *opcode)
{
    // Push the return address to the stack (it grows downward)
    // Then, jump to the given address
    uint16_t ret = state->pc + 3;
    state->memory[state->sp - 1] = (ret & 0xff00) >> 8;
    state->memory[state->sp - 2] = (ret & 0x00ff);
    state->sp -= 2;
    JMP(state, opcode);
}
void RET(State *state)
{
    // Return to the address stored in the stack (from CALL)
    state->pc = (state->memory[state->sp]) | (state->memory[state->sp + 1] << 8);
    state->sp += 2;
}
void RST(State *state, uint16_t addr)
{
    // Push the return address to stack
    uint16_t ret = state->pc + 1;
    state->memory[state->sp - 1] = (ret & 0xff00) >> 8;
    state->memory[state->sp - 2] = (ret & 0x00ff);
    state->sp -= 2;
    state->pc = addr;
}

void emulate_opcodes (State *state)
{
    unsigned char *opcode = &state->memory[state->pc];
    uint16_t m = (state->h << 8) | (state->l);

    switch (*opcode)
    {
        case 0x00: // NOP
            state->pc += 1;
            break;
        case 0x01: // LXI B,d16
            state->c = opcode[1];
            state->b = opcode[2];
            state->pc += 3;
            break;
        case 0x02: unimplemented_instruction(state); break;
        case 0x03: // INX B
            INX(state, &state->b, &state->c);
            break;
        case 0x04: // INR B
            INR(state, &state->b);
            break;
        case 0x05: // DCR B
            DCR(state, &state->b);
            break;
        case 0x06: unimplemented_instruction(state); break;
        case 0x07: unimplemented_instruction(state); break;
        case 0x08: unimplemented_instruction(state); break;
        case 0x09: // DAD B
            DAD(state, state->b, state->c);
            break;
        case 0x0a: unimplemented_instruction(state); break;
        case 0x0b: // DCX B
            DCX(state, &state->b, &state->c);
            break;
        case 0x0c: // INR C
            INR(state, &state->c);
            break;
        case 0x0d: // DCR C
            DCR(state, &state->c);
            break;
        case 0x0e: unimplemented_instruction(state); break;
        case 0x0f: unimplemented_instruction(state); break;

        case 0x10: unimplemented_instruction(state); break;
        case 0x11: unimplemented_instruction(state); break;
        case 0x12: unimplemented_instruction(state); break;
        case 0x13: // INX D
            INX(state, &state->d, &state->e);
            break;
        case 0x14: // INR D
            INR(state, &state->d);
            break;
        case 0x15: // DCR D
            DCR(state, &state->d);
            break;
        case 0x16: unimplemented_instruction(state); break;
        case 0x17: unimplemented_instruction(state); break;
        case 0x18: unimplemented_instruction(state); break;
        case 0x19: // DAD D
            DAD(state, state->d, state->e);
            break;
        case 0x1a: unimplemented_instruction(state); break;
        case 0x1b: // DCX D
            DCX(state, &state->d, &state->e);
            break;
        case 0x1c: // INR E
            INR(state, &state->e);
            break;
        case 0x1d: // DCR E
            DCR(state, &state->e);
            break;
        case 0x1e: unimplemented_instruction(state); break;
        case 0x1f: unimplemented_instruction(state); break;

        case 0x20: unimplemented_instruction(state); break;
        case 0x21: unimplemented_instruction(state); break;
        case 0x22: unimplemented_instruction(state); break;
        case 0x23: // INX H
            INX(state, &state->h, &state->l);
            break;
        case 0x24: // INR H
            INR(state, &state->h);
            break;
        case 0x25: // DCR H
            DCR(state, &state->h);
            break;
        case 0x26: unimplemented_instruction(state); break;
        case 0x27: unimplemented_instruction(state); break;
        case 0x28: unimplemented_instruction(state); break;
        case 0x29: // DAD H
            DAD(state, state->h, state->l);
            break;
        case 0x2a: unimplemented_instruction(state); break;
        case 0x2b: // DCX H
            DCX(state, &state->h, &state->l);
            break;
        case 0x2c: // INR L
            INR(state, &state->l);
            break;
        case 0x2d: // DCR L
            DCR(state, &state->l);
            break;
        case 0x2e: unimplemented_instruction(state); break;
        case 0x2f: unimplemented_instruction(state); break;

        case 0x30: unimplemented_instruction(state); break;
        case 0x31: unimplemented_instruction(state); break;
        case 0x32: unimplemented_instruction(state); break;
        case 0x33: // INX SP
            state->sp++;
            state->pc++;
            break;
        case 0x34: // INR M
            INR(state, &state->memory[m]);
            break;
        case 0x35: // DCR M
            DCR(state, &state->memory[m]);
            break;
        case 0x36: unimplemented_instruction(state); break;
        case 0x37: unimplemented_instruction(state); break;
        case 0x38: unimplemented_instruction(state); break;
        case 0x39: // DAD SP
            uint32_t hl = (state->h << 8) | (state->l);
            uint32_t ans = hl + (uint32_t) state->sp;
            state->flags.cy = (ans > 0xffff);
            state->h = (ans & 0xff00) >> 8;
            state->l = (ans & 0x00ff);
            state->pc++;
            break;
        case 0x3a: unimplemented_instruction(state); break;
        case 0x3b: // DCX SP
            state->sp--;
            state->pc++;
            break;
        case 0x3c: // INR A
            INR(state, &state->a);
            break;
        case 0x3d: // DCR A
            DCR(state, &state->a);
            break;
        case 0x3e: unimplemented_instruction(state); break;
        case 0x3f: unimplemented_instruction(state); break;

        case 0x40: unimplemented_instruction(state); break;
        case 0x41: unimplemented_instruction(state); break;
        case 0x42: unimplemented_instruction(state); break;
        case 0x43: unimplemented_instruction(state); break;
        case 0x44: unimplemented_instruction(state); break;
        case 0x45: unimplemented_instruction(state); break;
        case 0x46: unimplemented_instruction(state); break;
        case 0x47: unimplemented_instruction(state); break;
        case 0x48: unimplemented_instruction(state); break;
        case 0x49: unimplemented_instruction(state); break;
        case 0x4a: unimplemented_instruction(state); break;
        case 0x4b: unimplemented_instruction(state); break;
        case 0x4c: unimplemented_instruction(state); break;
        case 0x4d: unimplemented_instruction(state); break;
        case 0x4e: unimplemented_instruction(state); break;
        case 0x4f: unimplemented_instruction(state); break;

        case 0x50: unimplemented_instruction(state); break;
        case 0x51: unimplemented_instruction(state); break;
        case 0x52: unimplemented_instruction(state); break;
        case 0x53: unimplemented_instruction(state); break;
        case 0x54: unimplemented_instruction(state); break;
        case 0x55: unimplemented_instruction(state); break;
        case 0x56: unimplemented_instruction(state); break;
        case 0x57: unimplemented_instruction(state); break;
        case 0x58: unimplemented_instruction(state); break;
        case 0x59: unimplemented_instruction(state); break;
        case 0x5a: unimplemented_instruction(state); break;
        case 0x5b: unimplemented_instruction(state); break;
        case 0x5c: unimplemented_instruction(state); break;
        case 0x5d: unimplemented_instruction(state); break;
        case 0x5e: unimplemented_instruction(state); break;
        case 0x5f: unimplemented_instruction(state); break;

        case 0x60: unimplemented_instruction(state); break;
        case 0x61: unimplemented_instruction(state); break;
        case 0x62: unimplemented_instruction(state); break;
        case 0x63: unimplemented_instruction(state); break;
        case 0x64: unimplemented_instruction(state); break;
        case 0x65: unimplemented_instruction(state); break;
        case 0x66: unimplemented_instruction(state); break;
        case 0x67: unimplemented_instruction(state); break;
        case 0x68: unimplemented_instruction(state); break;
        case 0x69: unimplemented_instruction(state); break;
        case 0x6a: unimplemented_instruction(state); break;
        case 0x6b: unimplemented_instruction(state); break;
        case 0x6c: unimplemented_instruction(state); break;
        case 0x6d: unimplemented_instruction(state); break;
        case 0x6e: unimplemented_instruction(state); break;
        case 0x6f: unimplemented_instruction(state); break;

        case 0x70: unimplemented_instruction(state); break;
        case 0x71: unimplemented_instruction(state); break;
        case 0x72: unimplemented_instruction(state); break;
        case 0x73: unimplemented_instruction(state); break;
        case 0x74: unimplemented_instruction(state); break;
        case 0x75: unimplemented_instruction(state); break;
        case 0x76: unimplemented_instruction(state); break;
        case 0x77: unimplemented_instruction(state); break;
        case 0x78: unimplemented_instruction(state); break;
        case 0x79: unimplemented_instruction(state); break;
        case 0x7a: unimplemented_instruction(state); break;
        case 0x7b: unimplemented_instruction(state); break;
        case 0x7c: unimplemented_instruction(state); break;
        case 0x7d: unimplemented_instruction(state); break;
        case 0x7e: unimplemented_instruction(state); break;
        case 0x7f: unimplemented_instruction(state); break;

        case 0x80: // ADD B
            ADD(state, state->b);
            break;
        case 0x81: // ADD C
            ADD(state, state->c);
            break;
        case 0x82: // ADD D
            ADD(state, state->d);
            break;
        case 0x83: // ADD E
            ADD(state, state->e);
            break;
        case 0x84: // ADD H
            ADD(state, state->h);
            break;
        case 0x85: // ADD L
            ADD(state, state->l);
            break;
        case 0x86: // ADD M
            ADD(state, state->memory[m]);
            break;
            
        case 0x87: // ADD A
            ADD(state, state->a);
            break;
        case 0x88: // ADC B
            ADC(state, state->b);
            break;
        case 0x89: // ADC C
            ADC(state, state->c);
            break;
        case 0x8a: // ADC D
            ADC(state, state->d);
            break;
        case 0x8b: // ADC E
            ADC(state, state->e);
            break;
        case 0x8c: // ADC H
            ADC(state, state->h);
            break;
        case 0x8d: // ADC L
            ADC(state, state->l);
            break;
        case 0x8e: // ADC M
            ADC(state, state->memory[m]);
            break;
        case 0x8f: // ADC A
            ADC(state, state->a);
            break;

        case 0x90: // SUB B
            break;
        case 0x91: // SUB C
            SUB(state, state->c);
            break;
        case 0x92: // SUB D
            SUB(state, state->d);
            break;
        case 0x93: // SUB E
            SUB(state, state->e);
            break;
        case 0x94: // SUB H
            SUB(state, state->h);
            break;
        case 0x95: // SUB L
            SUB(state, state->l);
            break;
        case 0x96: // SUB M
            SUB(state, state->memory[m]);
            break;
        case 0x97: // SUB A
            SUB(state, state->a);
            break;
        case 0x98: // SBB B
            SBB(state, state->b);
            break;
        case 0x99: // SBB C
            SBB(state, state->c);
            break;
        case 0x9a: // SBB D
            SBB(state, state->d);
            break;
        case 0x9b: // SBB E
            SBB(state, state->e);
            break;
        case 0x9c: // SBB H
            SBB(state, state->h);
            break;
        case 0x9d: // SBB L
            SBB(state, state->l);
            break;
        case 0x9e: // SBB M
            SBB(state, state->memory[m]);
            break;
        case 0x9f: // SBB A
            SBB(state, state->a);
            break;

        case 0xa0: unimplemented_instruction(state); break;
        case 0xa1: unimplemented_instruction(state); break;
        case 0xa2: unimplemented_instruction(state); break;
        case 0xa3: unimplemented_instruction(state); break;
        case 0xa4: unimplemented_instruction(state); break;
        case 0xa5: unimplemented_instruction(state); break;
        case 0xa6: unimplemented_instruction(state); break;
        case 0xa7: unimplemented_instruction(state); break;
        case 0xa8: unimplemented_instruction(state); break;
        case 0xa9: unimplemented_instruction(state); break;
        case 0xaa: unimplemented_instruction(state); break;
        case 0xab: unimplemented_instruction(state); break;
        case 0xac: unimplemented_instruction(state); break;
        case 0xad: unimplemented_instruction(state); break;
        case 0xae: unimplemented_instruction(state); break;
        case 0xaf: unimplemented_instruction(state); break;

        case 0xb0: unimplemented_instruction(state); break;
        case 0xb1: unimplemented_instruction(state); break;
        case 0xb2: unimplemented_instruction(state); break;
        case 0xb3: unimplemented_instruction(state); break;
        case 0xb4: unimplemented_instruction(state); break;
        case 0xb5: unimplemented_instruction(state); break;
        case 0xb6: unimplemented_instruction(state); break;
        case 0xb7: unimplemented_instruction(state); break;
        case 0xb8: unimplemented_instruction(state); break;
        case 0xb9: unimplemented_instruction(state); break;
        case 0xba: unimplemented_instruction(state); break;
        case 0xbb: unimplemented_instruction(state); break;
        case 0xbc: unimplemented_instruction(state); break;
        case 0xbd: unimplemented_instruction(state); break;
        case 0xbe: unimplemented_instruction(state); break;
        case 0xbf: unimplemented_instruction(state); break;

        case 0xc0: // RNZ
            if (state->flags.z == 0)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xc1: unimplemented_instruction(state); break;
        case 0xc2: // JNZ a16
            if (state->flags.z == 0)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xc3: // JMP a16
            JMP(state, opcode);
            break;
        case 0xc4: // CNZ a16
            if (state->flags.z == 0)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xc5: unimplemented_instruction(state); break;
        case 0xc6: unimplemented_instruction(state); break;
        case 0xc7: unimplemented_instruction(state); break;
        case 0xc8: // RZ
            if (state->flags.z)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xc9: // RET
            RET(state);
            break;
        case 0xca: // JZ a16
            if (state->flags.z)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xcb: unimplemented_instruction(state); break;
        case 0xcc: // CZ a16
            if (state->flags.z)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xcd: // CALL a16
            CALL(state, opcode);
            break;
        case 0xce: unimplemented_instruction(state); break;
        case 0xcf: unimplemented_instruction(state); break;

        case 0xd0: // RNC
            if (state->flags.cy == 0)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xd1: unimplemented_instruction(state); break;
        case 0xd2: // JNC a16
            if (state->flags.cy == 0)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xd3: unimplemented_instruction(state); break;
        case 0xd4: // CNC a16
            if (state->flags.cy == 0)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xd5: unimplemented_instruction(state); break;
        case 0xd6: unimplemented_instruction(state); break;
        case 0xd7: unimplemented_instruction(state); break;
        case 0xd8: // RC
            if (state->flags.cy)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xd9: unimplemented_instruction(state); break;
        case 0xda: // JC a16
            if (state->flags.cy)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xdb: unimplemented_instruction(state); break;
        case 0xdc: // CC a16
            if (state->flags.cy)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xdd: unimplemented_instruction(state); break;
        case 0xde: unimplemented_instruction(state); break;
        case 0xdf: unimplemented_instruction(state); break;

        case 0xe0: // RPO
            if (state->flags.p == 0)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xe1: unimplemented_instruction(state); break;
        case 0xe2: // JPO a16
            if (state->flags.p == 0)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xe3: unimplemented_instruction(state); break;
        case 0xe4: // CPO a16
            if (state->flags.p == 0)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xe5: unimplemented_instruction(state); break;
        case 0xe6: unimplemented_instruction(state); break;
        case 0xe7: unimplemented_instruction(state); break;
        case 0xe8: // RPE
            if (state->flags.p)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xe9: // PCHL
            state->pc = ((state->h) << 8) | (state->l);
            break;
        case 0xea: // JPE a16
            if (state->flags.p)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xeb: unimplemented_instruction(state); break;
        case 0xec: // CPE a16
            if (state->flags.p)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xed: unimplemented_instruction(state); break;
        case 0xee: unimplemented_instruction(state); break;
        case 0xef: unimplemented_instruction(state); break;

        case 0xf0: // RP
            if (state->flags.z == 0)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xf1: unimplemented_instruction(state); break;
        case 0xf2: // JP a16
            if (state->flags.s == 0)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xf3: unimplemented_instruction(state); break;
        case 0xf4: // CP a16
            if (state->flags.s == 0)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xf5: unimplemented_instruction(state); break;
        case 0xf6: unimplemented_instruction(state); break;
        case 0xf7: unimplemented_instruction(state); break;
        case 0xf8: // RM
            if (state->flags.s)
            {
                RET(state);
            }
            else
            {
                state->pc += 1;
            }
            break;
        case 0xf9: unimplemented_instruction(state); break;
        case 0xfa:  // JM a16
            if (state->flags.s)
            {
                JMP(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xfb: unimplemented_instruction(state); break;
        case 0xfc: // CM a16
            if (state->flags.s)
            {
                CALL(state, opcode);
            }
            else
            {
                state->pc += 3;
            }
            break;
        case 0xfd: unimplemented_instruction(state); break;
        case 0xfe: unimplemented_instruction(state); break;
        case 0xff: unimplemented_instruction(state); break;   
    }
}
