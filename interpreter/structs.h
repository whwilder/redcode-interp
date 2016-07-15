#ifndef STRUCTS
#define STRUCTS
typedef struct CoreCell {
   int opcode;
   int arg1;
   int arg2;
   int mode_a;
   int mode_b;
   int modifier;
} CoreCell;

#ifndef CORE
#define CORE
CoreCell core[CORE_SIZE];
#endif


int pc[MAX_PROCESSES];
int *curr_pc;
int process_idx;
int num_processes;

void print_tok();

void addInstrs();
void run();
int mov( CoreCell instr );
int add( CoreCell instr );
int sub( CoreCell instr );
int mult( CoreCell instr );
int divd( CoreCell instr );
int mod( CoreCell instr );
int jmp( CoreCell instr );
int spl( CoreCell instr );
void print_instr( CoreCell instr );
char get_addr_char( int addr_mode );
int fetch_addr(int rel_addr, int addr_mode);
#endif
