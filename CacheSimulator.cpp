#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>

using namespace std;

enum {Load = 0, Store = 1};

static unsigned int accs[2] = {0, 0};
static unsigned int miss[2] = {0, 0};
static unsigned int **cache; //2D array of pointers
static unsigned int *FIFO;
static unsigned int byteoff;
static unsigned int wordoff;
static unsigned int index;
static unsigned int tag;
static unsigned int ARCH = 32;
static unsigned int **valid;

static void initCache(unsigned int sets, unsigned int blocksize, unsigned int assoc){

	cache = new unsigned int*[sets];
	valid = new unsigned int*[];
	byteoff = log2((blocksize/8));
	wordoff = log2((blocksize/ARCH));
	index = log2(((blocksize*ARCH)/assoc));
	tag = ARCH - (byteoff + wordoff + index);

	//initialize 2D array
	for (int i = 0; i < sets; ++i){
		cache[i] = new unsigned int[assoc];
	}

	for (int i = 0; i < sets; i++){
		for (int j = 0; j < assoc; j++){
			cache[i][j] = 0;
		}
	}

}

static void accessCache(unsigned int addr, unsigned int type){

  /*cache[index];
  for (int i = 0; i < ; ++i){
  	if (valid[index]  && tag==cache[index])
  		valid[index] = 0;
  }

  if (valid[index]==0){
  	accs[type]++;

  } else {

  }*/

}

static void finiCache(){
  printf("loads: %u\n", accs[Load]);
  printf("load misses: %u\n", miss[Load]);
  printf("stores: %u\n", accs[Store]);
  printf("store misses: %u\n", miss[Store]);

  if (accs[Load] > 0){
    printf("load hit ratio: %.2f%%\n", 100.0 * (accs[Load] - miss[Load]) / accs[Load]);
  }
  if (accs[Store] > 0){
    printf("store hit ratio: %.2f%%\n", 100.0 * (accs[Store] - miss[Store]) / accs[Store]);
  }
  if ((accs[Load] + accs[Store]) > 0){
    printf("overall hit ratio: %.2f%%\n", 100.0 * (accs[Load] + accs[Store] - miss[Load] - miss[Store]) / (accs[Load] + accs[Store]));
  }
}

int main(int argc, char* argv[]){
  printf("CS3339 Cache Simulator\n");

  if (argc != 5){
    printf("usage: %s trace_file number_of_sets block_size associativity\n", argv[0]);
    return -1;
  }

  unsigned int sets = atoi(argv[2]);
  if (sets == 0) {printf("number_of_sets must be greater than 0\n"); exit(-1);}
  if ((sets & (sets - 1)) != 0) {printf("number_of_sets must be a power of 2\n"); exit(-1);}
  unsigned int blocksize = atoi(argv[3]);
  if (blocksize == 0) {printf("block_size must be greater than 0\n"); exit(-1);}
  if ((blocksize & (blocksize - 1)) != 0) {printf("block_size must be a power of 2\n"); exit(-1);}
  unsigned int assoc = atoi(argv[4]);
  if (assoc == 0) {printf("associativity must be greater than 0\n"); exit(-1);}
  printf("using %u sets, %u-byte blocks, and %u-way associativity\n", sets, blocksize, assoc);

  FILE* f = fopen(argv[1], "rb");  assert(f != NULL);
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);  assert(size > 0);
  assert((size % sizeof(unsigned int)) == 0);
  size /= sizeof(int);
  unsigned int *data = new unsigned int[size];
  fseek(f, 0, SEEK_SET);
  size_t length = fread(data, sizeof(unsigned int), size, f);  assert(length == size);
  fclose(f);
  printf("trace file %s contains %lld entries\n", argv[1], (long long)length);

  initCache(sets, blocksize, assoc);

  size_t count = 0;
  for (size_t i = 0; i < length; i++){
    unsigned int addr = data[i] & ~1;
    unsigned int isStore = data[i] & 1;
    accessCache(addr, isStore);
  }

  finiCache();

  delete [] data;
  return 0;
}