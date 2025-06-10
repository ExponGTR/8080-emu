#include <stdio.h>
#include <stdlib.h>

/*    
*codebuffer is a valid pointer to 8080 assembly code    
pc is the current offset into the code
returns the number of bytes of the op    
*/    

int disassemble_op(FILE *f, unsigned char *codebuffer, int pc)
{
    unsigned char *code = &codebuffer[pc];
    int opbytes = 1;
    fprintf(f, "%04x ", pc);
    switch (*code)
    {
        // Reference: https://tobiasvl.github.io/optable/intel-8080/

        case 0x00: fprintf(f,"NOP"); break;
        case 0x01: fprintf(f,"LXI\tB,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x02: fprintf(f,"STAX\tB"); break;
        case 0x03: fprintf(f,"INX\tB"); break;
        case 0x04: fprintf(f,"INR\tB"); break;    
        case 0x05: fprintf(f,"DCR\tB"); break;    
        case 0x06: fprintf(f,"MVI\tB,#$%02x", code[1]); opbytes = 2; break;    
        case 0x07: fprintf(f,"RLC"); break;    
        case 0x08: fprintf(f,"NOP"); break; 
        case 0x09: fprintf(f,"DAD\tB"); break;
        case 0x0a: fprintf(f,"LDAX\tB"); break;
        case 0x0b: fprintf(f,"DCX\tB"); break;
        case 0x0c: fprintf(f,"INR\tC"); break;
        case 0x0d: fprintf(f,"DCR\tC"); break;
        case 0x0e: fprintf(f,"MVI\tC,#$%02x", code[1]); opbytes = 2; break;
        case 0x0f: fprintf(f,"RRC"); break;
        
        case 0x10: fprintf(f,"NOP"); break;
        case 0x11: fprintf(f,"LXI\tD,#%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x12: fprintf(f,"STAX\tD"); break;
        case 0x13: fprintf(f,"INX\tD"); break;
        case 0x14: fprintf(f,"INR\tD"); break;
        case 0x15: fprintf(f,"DCR\tD"); break;
        case 0x16: fprintf(f,"MVI\tD,#$%02x", code[1]); opbytes = 2; break;
        case 0x17: fprintf(f,"RAL"); break;
        case 0x18: fprintf(f,"NOP"); break;
        case 0x19: fprintf(f,"DAD\tD"); break;
        case 0x1a: fprintf(f,"LDAX\tD"); break;
        case 0x1b: fprintf(f,"DCX\tD"); break;
        case 0x1c: fprintf(f,"INR\tE"); break;
        case 0x1d: fprintf(f,"DCR\tE"); break;
        case 0x1e: fprintf(f,"MVI\tE,#%02x", code[1]); opbytes = 2; break;
        case 0x1f: fprintf(f,"RAR"); break;

        case 0x20: fprintf(f,"NOP"); break;
        case 0x21: fprintf(f,"LXI\tH,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x22: fprintf(f,"SHLD\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x23: fprintf(f,"INX\tH"); break;
        case 0x24: fprintf(f,"INR\tH"); break;
        case 0x25: fprintf(f,"DCR\tH"); break;
        case 0x26: fprintf(f,"MVI\tH,#$%02x", code[1]); opbytes = 2; break;
        case 0x27: fprintf(f,"DAA"); break;
        case 0x28: fprintf(f,"NOP"); break;
        case 0x29: fprintf(f,"DAD\tH"); break;
        case 0x2a: fprintf(f,"LHLD\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x2b: fprintf(f,"DCX\tH"); break;
        case 0x2c: fprintf(f,"INR\tL"); break;
        case 0x2d: fprintf(f,"DCR\tL"); break;
        case 0x2e: fprintf(f,"MVI\tL,#$%02x", code[1]); opbytes = 2; break;
        case 0x2f: fprintf(f,"CMA"); break;

        case 0x30: fprintf(f,"NOP"); break;
        case 0x31: fprintf(f,"LXI\tSP,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x32: fprintf(f,"STA\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x33: fprintf(f,"INX\tSP"); break;
        case 0x34: fprintf(f,"INR\tM"); break;
        case 0x35: fprintf(f,"DCR\tM"); break;
        case 0x36: fprintf(f,"MVI\tM,#$%02x", code[1]); opbytes = 2; break;
        case 0x37: fprintf(f,"STC"); break;
        case 0x38: fprintf(f,"NOP"); break;
        case 0x39: fprintf(f,"DAD\tSP"); break;
        case 0x3a: fprintf(f,"LDA\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x3b: fprintf(f,"DCX\tSP"); break;
        case 0x3c: fprintf(f,"INR\tA"); break;
        case 0x3d: fprintf(f,"DCR\tA"); break;
        case 0x3e: fprintf(f,"MVI\tA,#$%02x", code[1]); opbytes = 2; break;
        case 0x3f: fprintf(f,"CMC"); break;

        case 0x40: fprintf(f,"MOV\tB,B"); break;
        case 0x41: fprintf(f,"MOV\tB,C"); break;
        case 0x42: fprintf(f,"MOV\tB,D"); break;
        case 0x43: fprintf(f,"MOV\tB,E"); break;
        case 0x44: fprintf(f,"MOV\tB,H"); break;
        case 0x45: fprintf(f,"MOV\tB,L"); break;
        case 0x46: fprintf(f,"MOV\tB,M"); break;
        case 0x47: fprintf(f,"MOV\tB,A"); break;
        case 0x48: fprintf(f,"MOV\tC,B"); break;
        case 0x49: fprintf(f,"MOV\tC,C"); break;
        case 0x4a: fprintf(f,"MOV\tC,D"); break;
        case 0x4b: fprintf(f,"MOV\tC,E"); break;
        case 0x4c: fprintf(f,"MOV\tC,H"); break;
        case 0x4d: fprintf(f,"MOV\tC,L"); break;
        case 0x4e: fprintf(f,"MOV\tC,M"); break;
        case 0x4f: fprintf(f,"MOV\tC,A"); break;

        case 0x50: fprintf(f,"MOV\tD,B"); break;
        case 0x51: fprintf(f,"MOV\tD,C"); break;
        case 0x52: fprintf(f,"MOV\tD,D"); break;
        case 0x53: fprintf(f,"MOV\tD,E"); break;
        case 0x54: fprintf(f,"MOV\tD,H"); break;
        case 0x55: fprintf(f,"MOV\tD,L"); break;
        case 0x56: fprintf(f,"MOV\tD,M"); break;
        case 0x57: fprintf(f,"MOV\tD,A"); break;
        case 0x58: fprintf(f,"MOV\tE,B"); break;
        case 0x59: fprintf(f,"MOV\tE,C"); break;
        case 0x5a: fprintf(f,"MOV\tE,D"); break;
        case 0x5b: fprintf(f,"MOV\tE,E"); break;
        case 0x5c: fprintf(f,"MOV\tE,H"); break;
        case 0x5d: fprintf(f,"MOV\tE,L"); break;
        case 0x5e: fprintf(f,"MOV\tE,M"); break;
        case 0x5f: fprintf(f,"MOV\tE,A"); break;

        case 0x60: fprintf(f,"MOV\tH,B"); break;
        case 0x61: fprintf(f,"MOV\tH,C"); break;
        case 0x62: fprintf(f,"MOV\tH,D"); break;
        case 0x63: fprintf(f,"MOV\tH,E"); break;
        case 0x64: fprintf(f,"MOV\tH,H"); break;
        case 0x65: fprintf(f,"MOV\tH,L"); break;
        case 0x66: fprintf(f,"MOV\tH,M"); break;
        case 0x67: fprintf(f,"MOV\tL,A"); break;
        case 0x68: fprintf(f,"MOV\tL,B"); break;
        case 0x69: fprintf(f,"MOV\tL,C"); break;
        case 0x6a: fprintf(f,"MOV\tL,D"); break;
        case 0x6b: fprintf(f,"MOV\tL,E"); break;
        case 0x6c: fprintf(f,"MOV\tL,H"); break;
        case 0x6d: fprintf(f,"MOV\tL,L"); break;
        case 0x6e: fprintf(f,"MOV\tL,M"); break;
        case 0x6f: fprintf(f,"MOV\tL,A"); break;

        case 0x70: fprintf(f,"MOV\tM,B"); break;
        case 0x71: fprintf(f,"MOV\tM,C"); break;
        case 0x72: fprintf(f,"MOV\tM,D"); break;
        case 0x73: fprintf(f,"MOV\tM,E"); break;
        case 0x74: fprintf(f,"MOV\tM,H"); break;
        case 0x75: fprintf(f,"MOV\tM,L"); break;
        case 0x76: fprintf(f,"HLT"); break;
        case 0x77: fprintf(f,"MOV\tM,A"); break;
        case 0x78: fprintf(f,"MOV\tA,B"); break;
        case 0x79: fprintf(f,"MOV\tA,C"); break;
        case 0x7a: fprintf(f,"MOV\tA,D"); break;
        case 0x7b: fprintf(f,"MOV\tA,E"); break;
        case 0x7c: fprintf(f,"MOV\tA,H"); break;
        case 0x7d: fprintf(f,"MOV\tA,L"); break;
        case 0x7e: fprintf(f,"MOV\tA,M"); break;
        case 0x7f: fprintf(f,"MOV\tA,A"); break;

        case 0x80: fprintf(f,"ADD\tB"); break;
        case 0x81: fprintf(f,"ADD\tC"); break;
        case 0x82: fprintf(f,"ADD\tD"); break;
        case 0x83: fprintf(f,"ADD\tE"); break;
        case 0x84: fprintf(f,"ADD\tH"); break;
        case 0x85: fprintf(f,"ADD\tL"); break;
        case 0x86: fprintf(f,"ADD\tM"); break;
        case 0x87: fprintf(f,"ADD\tA"); break;
        case 0x88: fprintf(f,"ADC\tB"); break;
        case 0x89: fprintf(f,"ADC\tC"); break;
        case 0x8a: fprintf(f,"ADC\tD"); break;
        case 0x8b: fprintf(f,"ADC\tE"); break;
        case 0x8c: fprintf(f,"ADC\tH"); break;
        case 0x8d: fprintf(f,"ADC\tL"); break;
        case 0x8e: fprintf(f,"ADC\tM"); break;
        case 0x8f: fprintf(f,"ADC\tA"); break;

        case 0x90: fprintf(f,"SUB\tB"); break;
        case 0x91: fprintf(f,"SUB\tC"); break;
        case 0x92: fprintf(f,"SUB\tD"); break;
        case 0x93: fprintf(f,"SUB\tE"); break;
        case 0x94: fprintf(f,"SUB\tH"); break;
        case 0x95: fprintf(f,"SUB\tL"); break;
        case 0x96: fprintf(f,"SUB\tM"); break;
        case 0x97: fprintf(f,"SUB\tA"); break;
        case 0x98: fprintf(f,"SBB\tB"); break;
        case 0x99: fprintf(f,"SBB\tC"); break;
        case 0x9a: fprintf(f,"SBB\tD"); break;
        case 0x9b: fprintf(f,"SBB\tE"); break;
        case 0x9c: fprintf(f,"SBB\tH"); break;
        case 0x9d: fprintf(f,"SBB\tL"); break;
        case 0x9e: fprintf(f,"SBB\tM"); break;
        case 0x9f: fprintf(f,"SBB\tA"); break;

        case 0xa0: fprintf(f,"ANA\tB"); break;
        case 0xa1: fprintf(f,"ANA\tC"); break;
        case 0xa2: fprintf(f,"ANA\tD"); break;
        case 0xa3: fprintf(f,"ANA\tE"); break;
        case 0xa4: fprintf(f,"ANA\tH"); break;
        case 0xa5: fprintf(f,"ANA\tL"); break;
        case 0xa6: fprintf(f,"ANA\tM"); break;
        case 0xa7: fprintf(f,"ANA\tA"); break;
        case 0xa8: fprintf(f,"XRA\tB"); break;
        case 0xa9: fprintf(f,"XRA\tC"); break;
        case 0xaa: fprintf(f,"XRA\tD"); break;
        case 0xab: fprintf(f,"XRA\tE"); break;
        case 0xac: fprintf(f,"XRA\tH"); break;
        case 0xad: fprintf(f,"XRA\tL"); break;
        case 0xae: fprintf(f,"XRA\tM"); break;
        case 0xaf: fprintf(f,"XRA\tA"); break;

        case 0xb0: fprintf(f,"ORA\tB"); break;
        case 0xb1: fprintf(f,"ORA\tC"); break;
        case 0xb2: fprintf(f,"ORA\tD"); break;
        case 0xb3: fprintf(f,"ORA\tE"); break;
        case 0xb4: fprintf(f,"ORA\tH"); break;
        case 0xb5: fprintf(f,"ORA\tL"); break;
        case 0xb6: fprintf(f,"ORA\tM"); break;
        case 0xb7: fprintf(f,"ORA\tA"); break;
        case 0xb8: fprintf(f,"CMP\tB"); break;
        case 0xb9: fprintf(f,"CMP\tC"); break;
        case 0xba: fprintf(f,"CMP\tD"); break;
        case 0xbb: fprintf(f,"CMP\tE"); break;
        case 0xbc: fprintf(f,"CMP\tH"); break;
        case 0xbd: fprintf(f,"CMP\tL"); break;
        case 0xbe: fprintf(f,"CMP\tM"); break;
        case 0xbf: fprintf(f,"CMP\tA"); break;

        case 0xc0: fprintf(f,"RNZ"); break;
        case 0xc1: fprintf(f,"POP\tB"); break;
        case 0xc2: fprintf(f,"JNZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc3: fprintf(f,"JMP\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc4: fprintf(f,"CNZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc5: fprintf(f,"PUSH\tB"); break;
        case 0xc6: fprintf(f,"ADI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xc7: fprintf(f,"RST\t0"); break;
        case 0xc8: fprintf(f,"RZ"); break;
        case 0xc9: fprintf(f,"RET"); break;
        case 0xca: fprintf(f,"JZ \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcb: fprintf(f,"NOP"); break;
        case 0xcc: fprintf(f,"CZ \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcd: fprintf(f,"CALL\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xce: fprintf(f,"ACI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xcf: fprintf(f,"RST\t1"); break;

        case 0xd0: fprintf(f,"RNC"); break;
        case 0xd1: fprintf(f,"POP\tD"); break;
        case 0xd2: fprintf(f,"JNC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd3: fprintf(f,"OUT\t#$%02x", code[1]); opbytes = 2; break;
        case 0xd4: fprintf(f,"CNC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd5: fprintf(f,"PUSH\tD"); break;
        case 0xd6: fprintf(f,"SUI\t#%02x", code[1]); opbytes = 2; break;
        case 0xd7: fprintf(f,"RST\t2"); break;
        case 0xd8: fprintf(f,"RC"); break;
        case 0xd9: fprintf(f,"NOP"); break;
        case 0xda: fprintf(f,"JC \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdb: fprintf(f,"IN \t#$%02x", code[1]); opbytes = 2; break;
        case 0xdc: fprintf(f,"CC \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdd: fprintf(f,"NOP"); break;
        case 0xde: fprintf(f,"SBI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xdf: fprintf(f,"RST\t3"); break;

        case 0xe0: fprintf(f,"RPO"); break;
        case 0xe1: fprintf(f,"POP\tH"); break;
        case 0xe2: fprintf(f,"JPO\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe3: fprintf(f,"XTHL"); break;
        case 0xe4: fprintf(f,"CPO\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe5: fprintf(f,"PUSH\tH"); break;
        case 0xe6: fprintf(f,"ANI\t#%02x", code[1]); opbytes = 2; break;
        case 0xe7: fprintf(f,"RST\t4"); break;
        case 0xe8: fprintf(f,"RPE"); break;
        case 0xe9: fprintf(f,"PCHL"); break;
        case 0xea: fprintf(f,"JPE\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xeb: fprintf(f,"XCHG"); break;
        case 0xec: fprintf(f,"CPE\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xed: fprintf(f,"NOP"); break;
        case 0xee: fprintf(f,"XRI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xef: fprintf(f,"RST\t5"); break;
        
        case 0xf0: fprintf(f,"RP"); break;
        case 0xf1: fprintf(f,"POP\tPSW"); break;
        case 0xf2: fprintf(f,"JP \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf3: fprintf(f,"DI"); break;
        case 0xf4: fprintf(f,"CP \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf5: fprintf(f,"PUSH\tPSW"); break;
        case 0xf6: fprintf(f,"ORI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xf7: fprintf(f,"RST\t6"); break;
        case 0xf8: fprintf(f,"RM"); break;
        case 0xf9: fprintf(f,"SPHL"); break;
        case 0xfa: fprintf(f,"JM \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfb: fprintf(f,"EI");
        case 0xfc: fprintf(f,"CM \t$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfd: fprintf(f,"NOP"); break;
        case 0xfe: fprintf(f,"CPI\t#$%02x", code[1]); opbytes = 2; break;
        case 0xff: fprintf(f,"RST\t7"); break;

        default: fprintf(f,"Error: Undefined opcode $%02x", *code); break;

    }
    fprintf(f,"\n");
    return opbytes;
}

int main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        printf("Error: Could not open file.\n");
        exit(1);
    }

    // Reading file into a buffer
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    unsigned char *buffer = malloc(fsize);
    fread(buffer, fsize, 1, f);
    fclose(f);
    FILE *f1 = fopen("code.asm", "w");
    if (f == NULL)
    {
        printf("Error: Could not open file.\n");
        exit(1);
    }
    int pc = 0;
    while (pc < fsize)
    {
        pc += disassemble_op(f1, buffer, pc);
    }

    free(buffer);

    return 0;
}
