#include <stdlib.h>
#include <stdio.h>

const char reg[32][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
  "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3",
  "$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

static void decode(int pc, int instr)
{
  int opcode, rs, rt, rd, shamt, funct, uimm, simm, addr; //opcode source temp destination shift function unsigned signed address

  opcode = (unsigned)instr >> 26; //changed from 25 to 26, as opcode has 6 bits not 7
  rs = (instr >> 21) & 0x1f; //correct rs starts at 21
  rt = (instr >> 16) & 0x1f; //correct rt starts at 16
  rd = (instr >> 11) & 0x1f; //correct rd starts at 11
  shamt = (instr >> 6) & 0x1f; //correct shamt starts at 6
  funct = instr & 0x3f; //changed address 0x1f to 0x3f as this was 011111 and we need to mask it with 111111
  uimm = instr & 0xffff; //working fine
  simm = ((signed)uimm << 16) >> 16; //correct
  addr = instr & 0x3ffffff; //changed from 0x3fffffe to 0x3ffffff as this is our base address, masking error

  switch (opcode) {
    case 0x00:
      switch (funct) { //displays the op code for each mips function all are a word apart
        case 0x00: printf("%8x: sll %s, %s, %d\n", pc, reg[rd], reg[rs], shamt); break; //changed simm to shamt
        case 0x03: printf("%8x: sra %s, %s, %d\n", pc, reg[rd], reg[rs], shamt); break; //changed usim to shamt
        case 0x08: printf("%8x: jr %d\n", pc, reg[rs]); break; //correct
        case 0x10: printf("%8x: mfhi %s\n", pc, reg[rd]); break; //changed from rd to rs to accept source
        case 0x12: printf("%8x: mflo %s\n", pc, reg[rd]); break; //changed from rd to rs to accept source
        case 0x18: printf("%8x: mult %s, %s\n", pc, reg[rs], reg[rt]); break; //mult operand is incorrectly formatted, mul rs to rt and added mul to mult
        case 0x1a: printf("%8x: div %s, %s\n", pc, reg[rs], reg[rt]); break; //working
        case 0x21: printf("%8x: addu %s, %s, %s\n", pc, reg[rd], reg[rs], reg[rt]); break; //working
        case 0x23: printf("%8x: subu %s, %s, %s\n", pc, reg[rd], reg[rs], reg[rt]); break; //added a comma between the %s statements
        case 0x2a: printf("%8x: slt %s, %s, %s\n", pc, reg[rd], reg[rs], reg[rt]); break; //changed last reg statement from rd to rt as format was incorrect
        default: printf("%8x: unimplemented\n", pc);
      }
      break;
    case 0x02: printf("%8x: j %x\n", pc, ((pc+4) & 0xf0000000) + addr*4); break;
    case 0x03: printf("%8x: jal %x\n", pc, (pc+4) & 0xf0000000) + addr*42); break; //changed the offest from 4 to 2 as we were shifting too many bits
    case 0x04: printf("%8x: beq %s, %s, %x\n", pc, reg[rs], reg[rt], (pc+4) + (simm<<2)); break; //added parenthesis around order of operations for pc+word then signed shift 2 bits
    case 0x05: printf("%8x: bne %s, %s, %x\n", pc, reg[rs], reg[rt], (pc+4) + (simm<<2)); break; //added parenthesis around order of operations for pc+word then signed shift 2 bits
    case 0x08: printf("%8x: addi %s, %s, %d\n", pc, reg[rd], reg[rs], simm); break; //changed rt to rd: formatting for andi rd rs simm, 
    case 0x09: printf("%8x: addiu %s, %s, %d\n", pc, reg[rt], reg[rs], simm); break; //changed to addiu from addu which was incorrect instruction name
    case 0x0c: printf("%8x: andi %s, %s, %d\n", pc, reg[rs], reg[rs], simm); break; //changed rt to rs: formatting for addi rs rs simm
    case 0x0f: printf("%8x: lui %s, %d\n", pc, reg[rt], simm); break; //changed rd to rs to load temporary
    case 0x1a: printf("%8x: trap %x\n", pc, addr); break;
    case 0x23: printf("%8x: lw %s, %d(%s)\n", pc, reg[rt], simm, reg[rs]); break; //changed rs to rt and rt to rs: formatting for lw rt, simm(rs)
    case 0x2b: printf("%8x: sw %s, %d(%s)\n", pc, reg[rt], simm, reg[rs]); break; //changed simm to reg[rt], reg[rs] to simm
    default: printf("%8x: unimplemented\n", pc);
  }
}

static int convert(unsigned int x)
{
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

int main(int argc, char *argv[])
{
  int c, count, start, little_endian, *instruction;
  FILE *f;

  printf("CS3339 - MIPS Disassembler\n");
  if (argc != 2) {fprintf(stderr, "usage: %s mips_executable\n", argv[0]); exit(-1);}
  if (sizeof(int) != 4) {fprintf(stderr, "error: need 4-byte integers\n"); exit(-1);}

  count = 1;
  little_endian = *((char *)&count);

  f = fopen(argv[1], "r+b");
  if (f == NULL) {fprintf(stderr, "error: could not open file %s\n", argv[1]); exit(-1);}
  c = fread(&count, 4, 1, f);
  if (c != 1) {fprintf(stderr, "error: could not read count from file %s\n", argv[1]); exit(-1);}
  if (little_endian) {
    count = convert(count);
  }
  c = fread(&start, 4, 1, f);
  if (c != 1) {fprintf(stderr, "error: could not read start from file %s\n", argv[1]); exit(-1);}
  if (little_endian) {
    start = convert(start);
  }

  instruction = (int *)(malloc(count * 4));
  if (instruction == NULL) {fprintf(stderr, "error: out of memory\n"); exit(-1);}
  c = fread(instruction, 4, count, f);
  if (c != count) {fprintf(stderr, "error: could not read instructions from file %s\n", argv[1]); exit(-1);}
  fclose(f);

  if (little_endian) {
    for (c = 0; c < count; c++) {
      instruction[c] = convert(instruction[c]);
    }
  }

  for (c = 0; c < count; c++) {
    decode(start + c * 4, instruction[c]);
  }
}
