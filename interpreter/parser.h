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
} InstrNode;

enum opCode {DAT, MOV, ADD, SUB, MUL, DIV, MOD, JMP, JMZ, JMN, DJN, SPL, CMP, SEQ, SNE, SLT, LDP,   STP, NOP};
enum addr_mode {IMM, DIRECT, INDIR_A, INDIR_B, PREDEC_A, PREDEC_B, POSTINC_A, POSTINC_B};
enum modifier  {A, B, AB, BA, F, X, I, NONE};

typedef enum opCode OpCode;
typedef enum addr_mode addr_mode;
typedef enum modifier modifier;
