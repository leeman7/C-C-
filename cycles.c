/*###########################################################
# COMPUTER ARCHITECTURE
# Project: #3
# Date: Thu Oct 15 13:07:41 CDT 2015 
# Student: Lee Nardo (lrn12) and Aaron Vazquez (a_v12)
# Summary: This is the Cycles program programmed in C.
#  This program calculate the number of: Bubbles, Flushes,
#  and Cycles that take place in the program. The program
#  simulates a pipeline and keeps track of all the stages
#  as well as instructions that enter the pipeline. Once
#  the program completes it outputs the total number of
#  Bubbles, Flushes,and Cycles.
#
###########################################################
Group Members: Aaron Vazquez and Lee Nardo*/


#include <stdlib.h>
#include <stdio.h>

#define MEMSIZE 1048576

static int little_endian, icount, *instruction;
static int mem[MEMSIZE / 4];

//Variables
//Initializing global variables
static int flush = 0;
static int cycles = 0;
static int bubbles = 0;

//Arrays
int des[5] = { 0 };
int stage[5] = { 0 };

//Pipeline Handler
void stage_handler(int rd, int needed){
  //moving through our array
  int i;
  for(i=5; i>0; i--){
    des[i] = des[i-1];//set destination to previous
    stage[i] = stage[i-1]; //set state ready earlier in pipe
    stage[i]--;            //decrement avialable stage by one
  }
  //set our arrays to 0 and accept input
  des[0] = rd; //dest of next rd set to 0
  stage[0] = needed; //next available stage set to 0 
}

//Bubble Handler
void source(int rs, int latest){

  int nbubbles;
  int i,j;

  //check for if we need to bubble in pipeline
  for(i=0; i<6; i++){
    if(des[i] == rs){ //check if rs matches current destination
      if(latest < stage[i]){
        nbubbles = stage[i] - latest; //number of bubbles found using current stage - latest
        for(j=0; j < nbubbles; j++)
          stage_handler(-5, -5); //send current stage a bubble
        bubbles += nbubbles; //add number of bubbles to the bubble counter
        cycles += nbubbles; //increment cycles
        i+=nbubbles;  //amount of bubbles must be added at des[i] & loop to skip instructions
      }
    }
  }

}

static int Convert(unsigned int x){
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

static int Fetch(int pc){
  pc = (pc - 0x00400000) >> 2;
  if ((unsigned)pc >= icount) {
    fprintf(stderr, "instruction fetch out of range\n");
    exit(-1);
  }
  return instruction[pc];
}

static int LoadWord(int addr){
  if ((addr & 3) != 0){
    fprintf(stderr, "unaligned data access\n");
    exit(-1);
  }
  addr -= 0x10000000;
  if ((unsigned)addr >= MEMSIZE){
    fprintf(stderr, "data access out of range\n");
    exit(-1);
  }
  return mem[addr / 4];
}

static void StoreWord(int data, int addr){
  if ((addr & 3) != 0){
    fprintf(stderr, "unaligned data access\n");
    exit(-1);
  }
  addr -= 0x10000000;
  if ((unsigned)addr >= MEMSIZE){
    fprintf(stderr, "data access out of range\n");
    exit(-1);
  }
  mem[addr / 4] = data;
}

static void Interpret(int start){
  int instr, opcode, rs, rt, rd, shamt, funct, uimm, simm, addr;
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
    addr = instr & 0x3ffffff;

    switch (opcode){
      case 0x00:
        switch (funct){
          case 0x00: /* sll */ 
            reg[rd] = reg[rs]<<shamt;
            source(rs,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //sll, rd = rs<<shamt correct format for sll
          case 0x03: /* sra */
            reg[rd] = reg[rs]>>shamt;
            source(rs,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //sra, rd = rs>>shamt correct formal for sra
          case 0x08: /* jr */
            pc = reg[rs];
            flush++; //flush
            source(rs,1); //ID stage
            stage_handler(-5,-5); //Bubble
            break; //pc is equal to rs, correct format for jr
          case 0x10: /* mfhi */
            reg[rd] = hi; source(32,2);
            stage_handler(rd,2); break; //mfhi changed to reg[rd] = hi
          case 0x12: /* mflo */
            reg[rd] = lo;
            source(32,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //unchanged
          case 0x18: /* mult */ 
            wide = reg[rs]; 
            wide *= reg[rt]; 
            lo = wide & 0xffffffff; 
            hi = wide >> 32; 
            source(rs,2); //EX stage
            source(rt,2); //EX stage
            stage_handler(32,3); //MEM stage
            break;
          case 0x1a: /* div */
            if (reg[rt] == 0){
              fprintf(stderr, "division by zero: pc = 0x%x\n", pc - 4);
              cont = 0;
            } else { 
              lo = reg[rs] / reg[rt];
              hi = reg[rs] % reg[rt];
            }
            source(rs,2); //EX stage
            source(rt,2); //EX stage
            stage_handler(32,5); //WB stage
            break;
          case 0x21: /* addu */
            reg[rd] = reg[rs] + reg[rt];
            source(rs,2); //EX stage
            source(rt,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //addu, correct format rd = rs + rt
          case 0x23: /* subu */
            reg[rd] = reg[rs] - reg[rt];
            source(rs,2); //EX stage
            source(rt,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //subu, correct format rd = rs - rt
          case 0x2a: /* slt */
            if (reg[rs] < reg[rt]) reg[rd] = 1;
            else 
              reg[rd] = 0; 
            source(rs,2); //EX stage
            source(rt,2); //EX stage
            stage_handler(rd,2); //EX stage
            break; //slt, correct format rs<rt then: rd true else: rd false
          default: fprintf(stderr, "unimplemented instruction: pc = 0x%x\n", pc - 4); cont = 0;
        }
        break;
      case 0x02: /* j */
        pc = (pc & 0xf0000000)+addr*4;
        flush++; //FLush
        stage_handler(-5,-5); //Bubble
        break; //j correct format: pc = pc&mask +address*4
      case 0x03: /* jal */
        reg[31] = pc;
        pc = (pc & 0xf0000000)+addr*4;
        flush++; //Flush
        stage_handler(31,1); //ID Stage
        break; //unchanged
      case 0x04: /* beq */
        if (reg[rs] == reg[rt]){
          pc = (pc+simm*4);
          flush++; //Flush
        }
        source(rs,1); //ID stage
        source(rt,1); //ID stage
        stage_handler(-5,-5); //Bubble
        break; //beq, correct format rs == rt then: pc = pc+C*4 else: pc
      case 0x05: /* bne */
        if (reg[rs] != reg[rt]){
          pc = (pc+simm*4);
          flush++; //Flush
        }
        source(rs,1); //ID stage
        source(rt,1); //ID stage
        stage_handler(-5,-5); //Bubble
        break; //bne, correct format rs != rt then: pc = pc+C*4 else: pc
      case 0x09: /* addiu */
        reg[rt] = (reg[rs] + simm);
        source(rs,2); //EX stage
        stage_handler(rt,2); //EX stage
        break; //addiu, correct format rt = rs+C
      case 0x0c: /* andi */
        reg[rt] = (reg[rs] & uimm);
        source(rs,2); //EX stage
        stage_handler(rt,2); //EX stage
        break; //andi, correct format rt = rs&Unsigned
      case 0x0f: /* lui */
        reg[rt] = simm<<16;
        stage_handler(rt,2); //EX stage
        break; //li, correct format simm shift left 16
      case 0x1a: /* trap */
        switch (addr & 0xf) {
          case 0x00: printf("\n"); break;
          case 0x01: printf(" %d ", reg[rs]); source(rs,2); break; //EX stage
          case 0x05: printf("\n? "); fflush(stdout); scanf("%d", &reg[rt]); stage_handler(rt,2); break; //EX stage
          case 0x0a: cont = 0; break;
          default: fprintf(stderr, "unimplemented trap: pc = 0x%x\n", pc - 4); cont = 0;
        }
        break;
      case 0x23: /* lw */
        reg[rt] = LoadWord((reg[rs]+simm));
        source(rs,2); //EX stage
        stage_handler(rt,4); //MEM2 stage
        break;  //LoadWord function, rt = lw(rs+C)
      case 0x2b: /* sw */
        StoreWord(reg[rt], (reg[rs]+simm));
        source(rs,2); //EX stage
        source(rt,3); //MEM1 stage
        stage_handler(-5,-5); //Bubble
        break;  //StoreWord function, sw(rt, rs+C)
      default: fprintf(stderr, "unimplemented instruction: pc = 0x%x\n", pc - 4); cont = 0;
    }
  }

  printf("\nprogram finished at pc = 0x%x (%d instructions executed)\n\n", pc, count);

  cycles = cycles + count + flush + 5; //count the number of cycles 
  printf("cycles = %d\n", cycles); //print number of cycles
  printf("bubbles = %d\n", bubbles); //print number of bubbles
  printf("flush = %d\n", flush); //print number of flushes 
}

int main(int argc, char *argv[]){
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
  if (little_endian){
    for (c = 0; c < icount; c++) {
      instruction[c] = Convert(instruction[c]);
    }
  }

  printf("running %s\n\n", argv[1]);
  Interpret(start);
}
