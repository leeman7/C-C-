#include <stdlib.h>
#include <stdio.h>

#define MEMSIZE 1048576

static int little_endian, icount, *instruction;
static int mem[MEMSIZE / 4];

static int Convert(unsigned int x)
{
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

static int Fetch(int pc)
{
  pc = (pc - 0x00400000) >> 2;
  if ((unsigned)pc >= icount) {
    fprintf(stderr, "instruction fetch out of range\n");
    exit(-1);
  }
  return instruction[pc];
}

static int LoadWord(int addr)
{
  if ((addr & 3) != 0) {
    fprintf(stderr, "unaligned data access\n");
    exit(-1);
  }
  addr -= 0x10000000;
  if ((unsigned)addr >= MEMSIZE) {
    fprintf(stderr, "data access out of range\n");
    exit(-1);
  }
  return mem[addr / 4];
}

static void StoreWord(int data, int addr)
{
  if ((addr & 3) != 0) {
    fprintf(stderr, "unaligned data access\n");
    exit(-1);
  }
  addr -= 0x10000000;
  if ((unsigned)addr >= MEMSIZE) {
    fprintf(stderr, "data access out of range\n");
    exit(-1);
  }
  mem[addr / 4] = data;
}

static void Interpret(int start)
{
  int instr, opcode, rs, rt, rd, shamt, funct, uimm, simm, addr;
  int flush; 
  int pc, hi, lo;
  int reg[32];
  int cont = 1, count = 0, i;
  long long wide;

  lo = hi = 0;
  pc = start;
  for (i = 1; i < 32; i++) reg[i] = 0;
  reg[28] = 0x10008000;  // gp
  reg[29] = 0x10000000 + MEMSIZE;  // sp

  while (cont) {
    count++;
    instr = Fetch(pc);
    pc += 4;
    reg[0] = 0;  // $zero

    opcode = (unsigned)instr >> 26;
    rs = (instr >> 21) & 0x1f;
    rt = (instr >> 16) & 0x1f;
    rd = (instr >> 11) & 0x1f;
    shamt = (instr >> 6) & 0x1f;
    funct = instr & 0x3f;
    uimm = instr & 0xffff;
    simm = ((signed)uimm << 16) >> 16;
    addr = instr & 0x3ffffff

    switch (opcode) {
      case 0x00:
        switch (funct) {
          case 0x00: /* sll */ reg[rd] = reg[rs]<<shamt; break; //sll, rd = rs<<shamt correct format for sll
          case 0x03: /* sra */ reg[rd] = reg[rs]>>shamt; break; //sra, rd = rs>>shamt correct formal for sra
          case 0x08: /* jr */ pc = reg[rs]; break; //pc is equal to rs, correct format for jr
          case 0x10: /* mfhi */ reg[rd] = hi; break; //mfhi changed to reg[rd] = hi
          case 0x12: /* mflo */ reg[rd] = lo; break; //unchanged
          case 0x18: /* mult */ wide = reg[rs]; wide *= reg[rt]; lo = wide & 0xffffffff; hi = wide >> 32; break;
          case 0x1a: /* div */ if (reg[rt] == 0) {fprintf(stderr, "division by zero: pc = 0x%x\n", pc - 4); cont = 0;} else {lo = reg[rs] / reg[rt]; hi = reg[rs] % reg[rt];} break;
          case 0x21: /* addu */ reg[rd] = reg[rs] + reg[rt]; break; //addu, correct format rd = rs + rt
          case 0x23: /* subu */ reg[rd] = reg[rs] - reg[rt]; break; //subu, correct format rd = rs - rt
          case 0x2a: /* slt */ if (reg[rs] < reg[rt]) reg[rd] = 1; else reg[rd] = 0; break; //slt, correct format rs<rt then: rd true else: rd false
          default: fprintf(stderr, "unimplemented instruction: pc = 0x%x\n", pc - 4); cont = 0;
        }
        break;
      case 0x02: /* j */ pc = (pc & 0xf0000000)+addr*4; flush++; break; //j correct format: pc = pc&mask +address*4
      case 0x03: /* jal */ reg[31] = pc; pc = (pc & 0xf0000000)+addr*4; break; //unchanged
      case 0x04: /* beq */ reg[rs] == reg[rt] ? pc = (pc+simm*4) : pc; break; //beq, correct format rs == rt then: pc = pc+C*4 else: pc
      case 0x05: /* bne */ reg[rs] != reg[rt] ? pc = (pc+simm*4) : pc; break; //bne, correct format rs != rt then: pc = pc+C*4 else: pc
      case 0x09: /* addiu */ reg[rt] = (reg[rs] + simm); break; //addiu, correct format rt = rs+C
      case 0x0c: /* andi */ reg[rt] = (reg[rs] & uimm); break; //andi, correct format rt = rs&Unsigned
      case 0x0f: /* lui */ reg[rt] = simm<<16; break; //lui, correct format simm shift left 16
      case 0x1a: /* trap */
        switch (addr & 0xf) {
          case 0x00: printf("\n"); break;
          case 0x01: printf(" %d ", reg[rs]); break;
          case 0x05: printf("\n? "); fflush(stdout); scanf("%d", &reg[rt]); break;
          case 0x0a: cont = 0; break;
          default: fprintf(stderr, "unimplemented trap: pc = 0x%x\n", pc - 4); cont = 0;
        }
        break;
      case 0x23: /* lw */ reg[rt] = LoadWord((reg[rs]+simm)); break;  //LoadWord function, rt = lw(rs+C)
      case 0x2b: /* sw */ StoreWord(reg[rt], (reg[rs]+simm)); break;  //StoreWord function, sw(rt, rs+C)
      default: fprintf(stderr, "unimplemented instruction: pc = 0x%x\n", pc - 4); cont = 0;
    }
  }

  printf("\nprogram finished at pc = 0x%x (%d instructions executed)\n", pc, count);
}

int main(int argc, char *argv[])
{
  int c, start;
  FILE *f;

  printf("CS 3339 - MIPS Interpreter\n");
  if (argc != 2) {fprintf(stderr, "usage: %s executable\n", argv[0]); exit(-1);}
  if (sizeof(int) != 4) {fprintf(stderr, "error: need 4-byte integers\n"); exit(-1);}
  if (sizeof(long long) != 8) {fprintf(stderr, "error: need 8-byte long longs\n"); exit(-1);}

  c = 1;
  little_endian = *((char *)&c);
  f = fopen(argv[1], "rb");
  if (f == NULL) {fprintf(stderr, "error: could not open file %s\n", argv[1]); exit(-1);}
  c = fread(&icount, 4, 1, f);
  if (c != 1) {fprintf(stderr, "error: could not read count from file %s\n", argv[1]); exit(-1);}
  if (little_endian) {
    icount = Convert(icount);
  }
  c = fread(&start, 4, 1, f);
  if (c != 1) {fprintf(stderr, "error: could not read start from file %s\n", argv[1]); exit(-1);}
  if (little_endian) {
    start = Convert(start);
  }

  instruction = (int *)(malloc(icount * 4));
  if (instruction == NULL) {fprintf(stderr, "error: out of memory\n"); exit(-1);}
  c = fread(instruction, 4, icount, f);
  if (c != icount) {fprintf(stderr, "error: could not read (all) instructions from file %s\n", argv[1]); exit(-1);}
  fclose(f);
  if (little_endian) {
    for (c = 0; c < icount; c++) {
      instruction[c] = Convert(instruction[c]);
    }
  }

  printf("running %s\n\n", argv[1]);
  Interpret(start);
}
