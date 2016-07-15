#include "interpreter.hpp"
#include <iostream>
#include <iomanip>
#include <string>
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
         core_size(size), max_cycles(cycles), max_proc(proc), pc(proc), core(size){
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
   curr_pc = &pc[process_idx];
   while( cycles < MAX_CYCLES ){
      InstrNode instr = core[*curr_pc];
      print_instr(instr);
      int err = 0;
      switch( instr.opcode ){
         case DAT:
            dat( core[*curr_pc] );
            break;
         case MOV:
            err = mov( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case ADD:
            err = add( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case SUB:
            err = sub( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case MUL:
            err = mult( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case DIV:
            err = divd( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case MOD:
            err = mod( core[*curr_pc] );
            (*curr_pc)++;
            break;
         case JMP:
            *curr_pc = jmp( core[*curr_pc] );
            break;
         case SPL:
            spl( core[*curr_pc] );
            (*curr_pc)++;
            break;
         default:
            break;
      }
      if( instr.opcode == DAT || err != 0)
         break;
      cycles++;
      process_idx = (process_idx + 1) % num_processes;
      *curr_pc = pc[process_idx];
      *curr_pc %= core.size();
   }
}

void Core::dat( InstrNode instr ){
   pc.erase(pc.begin() + process_idx);
   num_processes--;
}

int Core::mov(InstrNode instr){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch(instr.modifier){
      case NONE:
      case I:
         core[b].opcode   = core[a].opcode;
         core[b].arg1     = core[a].arg1;
         core[b].arg2     = core[a].arg2;
         core[b].mode_a   = core[a].mode_a;
         core[b].mode_b   = core[a].mode_b;
         core[b].modifier = core[a].modifier;
      break;
   }
   return 0;
}

int Core::add( InstrNode instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = (core[a].arg1+core[b].arg1) % core.size();
         break;
   }
   return 0;
}

int Core::sub( InstrNode instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = (core[a].arg1-core[b].arg1) % core.size();
         break;
   }
   return 0;
}

int Core::mult( InstrNode instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 * core[b].arg1 ) % core.size();
         break;
   }
   return 0;
}

int Core::divd( InstrNode instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[b].arg2 == 0)
      return 1;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 / core[b].arg1 ) % core.size();
         break;
   }
   return 0;
}

int Core::mod( InstrNode instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[b].arg2 == 0)
      return 1;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 % core[b].arg1 ) % core.size();
         break;
   }
   return 0;
}

int Core::jmp( InstrNode instr ){
   int a = fetch_addr( instr.arg1, instr.mode_a);
   switch( instr.modifier ){
      case NONE:
         return  a % core.size();
         break;
   }
   return 0;
}

int Core::spl( InstrNode instr ){
   int a = fetch_addr( instr.arg1, instr.mode_a);
   if (num_processes == MAX_PROCESSES)
      return *curr_pc;
   switch( instr.modifier ){
      case NONE:
         pc.push_back(core[a].arg1);
         num_processes++;
         break;
   }
   return *curr_pc;
}

int Core::fetch_addr(int rel_addr, int addr_mode){
   switch(addr_mode){
      case IMM:
         return *curr_pc;
         break;
      case NONE:
      case DIRECT:
         return (*curr_pc + rel_addr) % core.size();
         break;
      case INDIR_A:
         return (*curr_pc + rel_addr + core[*curr_pc+rel_addr].arg1) % core.size();
         break;
      case INDIR_B:
         return (*curr_pc + rel_addr + core[*curr_pc+rel_addr].arg2) % core.size();
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
   return *curr_pc;
}


void Core::print_instr(InstrNode instr){
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
         arg1 = (*(curr_pc) + instr.arg1) % 8000 - *(curr_pc);
         arg2 = -1;
         break;
      default:
         // if command is not recognized, halt and catch fire
         command = "HCF";
         break;
   }
   cout << setw(4) << right << *(curr_pc) <<": " << command << " "
                   << get_addr_char(instr.mode_a) << arg1; 
   if (arg2 > 0)
      cout << ", " << get_addr_char(instr.mode_b) << arg2;
   cout << endl;
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
