#ifndef STRUCTS
#define STRUCTS
#define CORE_SIZE 8000
#define MAX_CYCLES 8000
#define MAX_PROCESSES 8000
typedef struct InstrNode {
   int opcode;
   int arg1;
   int arg2;
   int mode_a;
   int mode_b;
   int modifier;
   struct InstrNode *next;
} InstrNode;

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

InstrNode *nodes;
InstrNode *lastNode;

enum opCode {DAT, MOV, ADD, SUB, MUL, DIV, MOD, JMP, JMZ, DJN, SPL, CMP, SEQ, SNE, SLT, LDP, STP, NOP};
enum addr_mode {IMM, DIRECT, INDIR_A, INDIR_B, PREDEC_A, PREDEC_B, POSTINC_A, POSTINC_B};
enum modifier  {A, B, AB, BA, F, X, I, NONE};
typedef enum opCode OpCode;
typedef enum addr_mode addr_mode;
typedef enum modifier modifier;

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
