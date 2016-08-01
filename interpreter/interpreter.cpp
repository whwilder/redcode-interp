#include "interpreter.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

extern std::vector<InstrNode> nodes;

void Core::addInstrs(std::vector<InstrNode> instrs){
   //int tmpPC = pc[0];
   std::copy(std::begin(instrs), std::end(instrs), std::begin(core));
   //for(std::vector<InstrNode>::iterator i = nodes.begin(); i != nodes.end(); i++){
   //   core[tmpPC].opcode = *nodes[i].opcode;
   //   core[tmpPC].arg1 = *nodes[i].arg1;
   //   core[tmpPC].arg2 = *nodes[i].arg2;

   //   core[tmpPC].modifier = *nodes[i].modifier;
   //   core[tmpPC].mode_a = *nodes[i].mode_a;
   //   core[tmpPC].mode_b *= nodes[i].mode_b;

   //   tmpPC++;
   //}
}

Core::Core(int size, int cycles, int proc) : 
         core_size(size), max_cycles(cycles), max_proc(proc), core(size){
   for(int i = 0; i < core_size; i++){
      core[i].opcode = 0;
      core[i].arg1 = 0;
      core[i].arg2 = 0;
      core[i].mode_a = 0;
      core[i].mode_b = 0;
      core[i].modifier = 0;

   }
}

void Core::run(){
   int cycles = 0;
   while( cycles < MAX_CYCLES && !proc_queue.empty() ){
      pc = proc_queue.front();
      proc_queue.pop();
      InstrNode instr = core[pc];
      //print_instr(instr);
      int err = 0;
      switch( instr.opcode ){
         case DAT:
            err = 1;
            break;
         case MOV:
            err = mov( core[pc] );
            (pc)++;
            break;
         case ADD:
            err = add( core[pc] );
            (pc)++;
            break;
         case SUB:
            sub( core[pc] );
            (pc)++;
            break;
         case MUL:
            mult( core[pc] );
            (pc)++;
            break;
         case DIV:
            err = divd( core[pc] );
            (pc)++;
            break;
         case MOD:
            err = mod( core[pc] );
            (pc)++;
            break;
         case JMP:
            pc = jmp( core[pc] );
            break;
         case JMZ:
            pc = jmz( core[pc] );
            break;
         case JMN:
            pc = jmn( core[pc] );
            break;
         case DJN:
            pc = djn( core[pc] );
            break;
         case SPL:
            spl( core[pc] );
            pc++;
            break;
         case SEQ:
            //pc = seq( core[pc] );
            break;
         default:
            break;
      }
      /*
       * We only push the pc back on to the queue if we haven't run into an error
       * (DAT instruction or division by 0)
       */
      if( instr.opcode != DAT || err == 0){
         pc %= core.size();
         proc_queue.push(pc);
      }
      cycles++;
   }
}

void Core::dump(){
   std::vector<InstrNode>::iterator it = core.begin();
   for( ; it != core.end(); it++ )
      print_instr(*it, it - core.begin() );
}

int Core::mov(InstrNode instr){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   switch(instr.modifier){
      case NONE:
      case I:
         core[addr_b].opcode   = core[addr_a].opcode;
         core[addr_b].arg1     = core[addr_a].arg1;
         core[addr_b].arg2     = core[addr_a].arg2;
         core[addr_b].mode_a   = core[addr_a].mode_a;
         core[addr_b].mode_b   = core[addr_a].mode_b;
         core[addr_b].modifier = core[addr_a].modifier;
         break;
      case A:
         core[addr_b].arg1     = core[addr_a].arg1;
         break;
      case B:
         core[addr_b].arg2     = core[addr_a].arg2;
         break;
      case AB:
         core[addr_b].arg2     = core[addr_a].arg1;
         break;
      case BA:
         core[addr_b].arg1     = core[addr_a].arg2;
         break;
      case F:
         core[addr_b].arg1     = core[addr_a].arg1;
         core[addr_b].arg2     = core[addr_a].arg2;
         break;
      case X:
         core[addr_b].arg2     = core[addr_a].arg1;
         core[addr_b].arg1     = core[addr_a].arg2;
         break;
   }
   return 0;
}

int Core::add( InstrNode instr ){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[addr_b].arg2 = ( core[addr_a].arg1 + core[addr_b].arg2 ) % core.size();
         break;
      case A:
         core[addr_b].arg1 = ( core[addr_a].arg1 + core[addr_b].arg1 ) % core.size();
         break;
      case B:
         core[addr_b].arg2 = ( core[addr_a].arg2 + core[addr_b].arg2 ) % core.size();
         break;
      case BA:
         core[addr_b].arg1 = ( core[addr_a].arg2 + core[addr_b].arg1 ) % core.size();
         break;
      case I:
      case F:
         core[addr_b].arg1 = ( core[addr_a].arg1 + core[addr_b].arg1 ) % core.size();
         core[addr_b].arg2 = ( core[addr_a].arg2 + core[addr_b].arg2 ) % core.size();
         break;
      case X:
         core[addr_b].arg2 = ( core[addr_a].arg1 + core[addr_b].arg1 ) % core.size();
         core[addr_b].arg1 = ( core[addr_a].arg2 + core[addr_b].arg2 ) % core.size();
         break;
   }
   return 0;
}

int Core::sub( InstrNode instr ){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[addr_b].arg2 = ( core[addr_a].arg1 - core[addr_b].arg2 ) % core.size();
         break;
      case A:
         core[addr_b].arg1 = ( core[addr_a].arg1 - core[addr_b].arg1 ) % core.size();
         break;
      case B:
         core[addr_b].arg2 = ( core[addr_a].arg2 - core[addr_b].arg2 ) % core.size();
         break;
      case BA:
         core[addr_b].arg1 = ( core[addr_a].arg2 - core[addr_b].arg1 ) % core.size();
         break;
      case I:
      case F:
         core[addr_b].arg1 = ( core[addr_a].arg1 - core[addr_b].arg1 ) % core.size();
         core[addr_b].arg2 = ( core[addr_a].arg2 - core[addr_b].arg2 ) % core.size();
         break;
      case X:
         core[addr_b].arg2 = ( core[addr_a].arg1 - core[addr_b].arg1 ) % core.size();
         core[addr_b].arg1 = ( core[addr_a].arg2 - core[addr_b].arg2 ) % core.size();
         break;
   }
   return 0;
}

int Core::mult( InstrNode instr ){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[addr_b].arg2 = ( core[addr_a].arg1 * core[addr_b].arg2 ) % core.size();
         break;
      case A:
         core[addr_b].arg1 = ( core[addr_a].arg1 * core[addr_b].arg1 ) % core.size();
         break;
      case B:
         core[addr_b].arg2 = ( core[addr_a].arg2 * core[addr_b].arg2 ) % core.size();
         break;
      case BA:
         core[addr_b].arg1 = ( core[addr_a].arg2 * core[addr_b].arg1 ) % core.size();
         break;
      case I:
      case F:
         core[addr_b].arg1 = ( core[addr_a].arg1 * core[addr_b].arg1 ) % core.size();
         core[addr_b].arg2 = ( core[addr_a].arg2 * core[addr_b].arg2 ) % core.size();
         break;
      case X:
         core[addr_b].arg2 = ( core[addr_a].arg1 * core[addr_b].arg1 ) % core.size();
         core[addr_b].arg1 = ( core[addr_a].arg2 * core[addr_b].arg2 ) % core.size();
         break;
   }
   return 0;
}

int Core::divd( InstrNode instr ){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[addr_b].arg2 == 0)
      return 2;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[addr_b].arg2 = ( core[addr_a].arg1 / core[addr_b].arg2 ) % core.size();
         break;
      case A:
         core[addr_b].arg1 = ( core[addr_a].arg1 / core[addr_b].arg1 ) % core.size();
         break;
      case B:
         core[addr_b].arg2 = ( core[addr_a].arg2 / core[addr_b].arg2 ) % core.size();
         break;
      case BA:
         core[addr_b].arg1 = ( core[addr_a].arg2 / core[addr_b].arg1 ) % core.size();
         break;
      case I:
      case F:
         core[addr_b].arg1 = ( core[addr_a].arg1 / core[addr_b].arg1 ) % core.size();
         core[addr_b].arg2 = ( core[addr_a].arg2 / core[addr_b].arg2 ) % core.size();
         break;
      case X:
         core[addr_b].arg2 = ( core[addr_a].arg1 / core[addr_b].arg1 ) % core.size();
         core[addr_b].arg1 = ( core[addr_a].arg2 / core[addr_b].arg2 ) % core.size();
         break;
   }
   return 0;
}

int Core::mod( InstrNode instr ){
   int addr_a = fetch_addr(instr.arg1, instr.mode_a);
   int addr_b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[addr_b].arg2 == 0)
      return 2;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[addr_b].arg2 = ( core[addr_a].arg1 % core[addr_b].arg2 ) % core.size();
         break;
      case A:
         core[addr_b].arg1 = ( core[addr_a].arg1 % core[addr_b].arg1 ) % core.size();
         break;
      case B:
         core[addr_b].arg2 = ( core[addr_a].arg2 % core[addr_b].arg2 ) % core.size();
         break;
      case BA:
         core[addr_b].arg1 = ( core[addr_a].arg2 % core[addr_b].arg1 ) % core.size();
         break;
      case I:
      case F:
         core[addr_b].arg1 = ( core[addr_a].arg1 % core[addr_b].arg1 ) % core.size();
         core[addr_b].arg2 = ( core[addr_a].arg2 % core[addr_b].arg2 ) % core.size();
         break;
      case X:
         core[addr_b].arg2 = ( core[addr_a].arg1 % core[addr_b].arg1 ) % core.size();
         core[addr_b].arg1 = ( core[addr_a].arg2 % core[addr_b].arg2 ) % core.size();
         break;
   }
   return 0;
}

int Core::jmp( InstrNode instr ){
   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
   /* 
    * JMP ignores whatever modifier it might have, so the pc simply becomes whatever is in 
    * the A-field ( as dictated by the address mode )
    */
   return  addr_a % core.size();
}

int Core::jmz( InstrNode instr ){
   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
   int addr_b = fetch_addr( instr.arg2, instr.mode_a);
   int cmp = core[addr_b].arg2;
   int ret_addr = pc+1;
   switch( instr.modifier ){
      case NONE:
      case AB:
      case B:
         if( cmp == 0)
            ret_addr =  addr_a % core.size();
         break;
      case X:
      case I:
      case F:
         if (core[addr_a].arg1 == 0 && cmp == 0 )
            ret_addr = addr_a % core.size();
         break;
   }
   return ret_addr;
}

int Core::jmn( InstrNode instr ){
   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
   int addr_b = fetch_addr( instr.arg2, instr.mode_a);
   int cmp = core[addr_b].arg2;
   int ret_addr = pc+1;
   switch( instr.modifier ){
      case NONE:
      case AB:
      case B:
         if( cmp != 0)
            ret_addr =  addr_a % core.size();
         break;
      case X:
      case I:
      case F:
         if (core[addr_a].arg1 != 0 && cmp != 0 )
            ret_addr = addr_a % core.size();
         break;
   }
   return ret_addr;
}

int Core::djn( InstrNode instr ){
   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
   int addr_b = fetch_addr( instr.arg2, instr.mode_a);
   int cmp = core[addr_b].arg2;
   int ret_addr = pc+1;
   switch( instr.modifier ){
      case AB:
      case B:
         core[addr_b].arg2--;
         if( cmp != 0)
            ret_addr =  addr_a % core.size();
         break;
      case X:
         break;
      case I:
      case F:
         core[addr_a].arg1--;
         core[addr_b].arg2--;
         if (core[addr_b].arg1 != 0 && core[addr_b].arg2 != 0 )
            ret_addr = addr_a % core.size();
         break;
   }
   return ret_addr;
}

int Core::spl( InstrNode instr ){
   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
   if (num_processes == MAX_PROCESSES)
      return pc;
   switch( instr.modifier ){
      case B:
         proc_queue.push(core[addr_a].arg1);
         break;
   }
   return pc;
}

//int Core::seq( InstrNode instr ){
//   int addr_a = fetch_addr( instr.arg1, instr.mode_a);
//   int addr_b = fetch_addr( instr.arg2, instr.mode_b);
//   ret_addr = pc + 1;
//   switch( instr.modifier ){
//      case NONE:
//         if( core[addr_a].arg1 == core[addr_b].arg2){
//            ret_addr++;
//         }
//         break;
//   }
//   return ret_addr;
//}

int Core::fetch_addr(int rel_addr, int addr_mode){
   switch(addr_mode){
      case IMM:
         return pc;
         break;
      case NONE:
      case DIRECT:
         return (pc + rel_addr) % core.size();
         break;
      case INDIR_A:
         return (pc + rel_addr + core[pc+rel_addr].arg1) % core.size();
         break;
      case INDIR_B:
         return (pc + rel_addr + core[pc+rel_addr].arg2) % core.size();
         break;
/* Leave these alone till I can figure out how the fuck they're supposed to work
      case PREDEC_A:
         return (pc + rel_addr + core[pc+rel_addr].arg1)-- % core.size();
         break;
      case PREDEC_B:
         return (pc + rel_addr + core[pc+rel_addr].arg2)-- % core.size();
         break;
      case POSTINC_A:
         return (pc + rel_addr + core[pc+rel_addr].arg1)++ % core.size();
         break;
      case POSTINC_B:
         return (pc + rel_addr + core[pc+rel_addr].arg2)++ % core.size();
         break;
*/
   }
   return pc;
}


void Core::print_instr(InstrNode instr, int addr){
   std::string command;
   int arg1 = instr.arg1;
   int arg2 = instr.arg2;
   switch( instr.opcode ){
      case DAT:
         command = "DAT";
         break;
      case MOV:
         command = "MOV";
         break;
      case ADD:
         command = "ADD";
         break;
      case JMP:
         command = "JMP";
         arg1 = (pc + instr.arg1) % 8000 - pc;
         arg2 = -1;
         break;
      case SPL:
         command = "SPL";
         arg1 = (pc + instr.arg1) % 8000 - pc;
         arg2 = -1;
         break;
      case JMZ:
         command = "JMZ";
         arg1 = (pc + instr.arg1) % 8000 - pc;
         arg2 = (pc + instr.arg2) % 8000 - pc;
         break;
      case JMN:
         command = "JMN";
         arg1 = (pc + instr.arg1) % 8000 - pc;
         arg2 = (pc + instr.arg2) % 8000 - pc;
         break;
      case DJN:
         command = "DJN";
         arg1 = (pc + instr.arg1) % 8000 - pc;
         arg2 = (pc + instr.arg2) % 8000 - pc;
         break;
      default:
         // if command is not recognized, halt and catch fire
         command = "HCF";
         break;
   }
   cout << setw(4) << right << addr <<": " << command << "." << get_modifier(instr.modifier) << " "
                   << get_addr_char(instr.mode_a) << arg1; 
   if (arg2 > -1)
      cout << ", " << get_addr_char(instr.mode_b) << arg2;
   cout << ": " << process_idx;
   cout << endl;
}

std::string Core::get_modifier( int modifier ){
   switch( modifier ){
      case A:
         return "A";
         break;
      case B:
         return "B";
         break;
      case AB:
         return "AB";
         break;
      case BA:
         return "BA";
         break;
      case I:
         return "I";
         break;
      case F:
         return "F";
         break;
      case X:
         return "X";
         break;
   }
   return "U";
}

char Core::get_addr_char(int addr_mode){
   switch(addr_mode){
      case IMM:
         return '#';
         break;
      case DIRECT:
         return '$';
         break;
      case INDIR_A:
         return '*';
         break;
      case INDIR_B:
         return '@';
         break;
      default:
         return ' ';
         break;
   }
}
