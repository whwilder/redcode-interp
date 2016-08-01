#include <vector>
#include <queue>
#include "parser.h"
#include <string>

//typedef struct InstrNode {
//   int opcode;
//   int arg1;
//   int arg2;
//   int mode_a;
//   int mode_b;
//   int modifier;
//
//} InstrNode;

class Core{
public:
   int pc;
   int process_idx;
   int core_size;
   int max_cycles;
   int max_proc;
   std::queue<int> proc_queue;
   std::vector<InstrNode> core;
   int num_processes;
   Core(int size, int max_cycles, int max_proc);

   void addInstrs(std::vector<InstrNode> instrs);
   void run();
   void dump();
   void dat( InstrNode instr );
   int mov( InstrNode instr );
   int add( InstrNode instr );
   int sub( InstrNode instr );
   int mult( InstrNode instr );
   int divd( InstrNode instr );
   int mod( InstrNode instr );
   int jmp( InstrNode instr );
   int jmz( InstrNode instr );
   int jmn( InstrNode instr );
   int djn( InstrNode instr );
   int spl( InstrNode instr );
   void print_instr( InstrNode instr, int addr );
   std::string get_modifier( int modifier );
   char get_addr_char( int addr_mode );
   int fetch_addr(int rel_addr, int addr_mode);

};
