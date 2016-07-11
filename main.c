#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "red.tab.h"

int main(int argc, char **argv){
   nodes = malloc(sizeof(InstrNode));
   nodes->next = NULL;
   lastNode = nodes;
   yyparse();
   InstrNode *tmp = nodes;
   nodes = nodes->next;
   free(tmp);
   
   pc[0] = 0;
   process_idx = 0;
   num_processes = 1;
   addInstrs();
   run();
}

void run(){
   int cycles = 0;
   curr_pc = &pc[process_idx];
   while( cycles < MAX_CYCLES ){
      CoreCell instr = core[*curr_pc];
      print_instr(instr);
      int err = 0;
      switch( instr.opcode ){
         case DAT:
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
      *curr_pc %= CORE_SIZE;
   }
}

int mov(CoreCell instr){
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

int add( CoreCell instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = (core[a].arg1+core[b].arg1) % CORE_SIZE;
         break;
   }
   return 0;
}

int sub( CoreCell instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = (core[a].arg1-core[b].arg1) % CORE_SIZE;
         break;
   }
   return 0;
}

int mult( CoreCell instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 * core[b].arg1 ) % CORE_SIZE;
         break;
   }
   return 0;
}

int divd( CoreCell instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[b].arg2 == 0)
      return 1;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 / core[b].arg1 ) % CORE_SIZE;
         break;
   }
   return 0;
}

int mod( CoreCell instr ){
   int a = fetch_addr(instr.arg1, instr.mode_a);
   int b = fetch_addr(instr.arg2, instr.mode_b);
   if( core[b].arg2 == 0)
      return 1;
   switch( instr.modifier ){
      case NONE:
      case AB:
         core[b].arg2 = ( core[a].arg1 % core[b].arg1 ) % CORE_SIZE;
         break;
   }
   return 0;
}

int jmp( CoreCell instr ){
   int a = fetch_addr( instr.arg1, instr.mode_a);
   switch( instr.modifier ){
      case NONE:
         return  a % CORE_SIZE;
         break;
   }
   return 0;
}

int spl( CoreCell instr ){
   int a = fetch_addr( instr.arg1, instr.mode_a);
   if (num_processes == MAX_PROCESSES)
      return *curr_pc;
   switch( instr.modifier ){
      case NONE:
         pc[num_processes] = core[a].arg1;
         num_processes++;
         break;
   }
   return *curr_pc;
}

int fetch_addr(int rel_addr, int addr_mode){
   switch(addr_mode){
      case IMM:
         return *curr_pc;
         break;
      case NONE:
      case DIRECT:
         return (*curr_pc + rel_addr) % CORE_SIZE;
         break;
      case INDIR_A:
         return (*curr_pc + rel_addr + core[*curr_pc+rel_addr].arg1) % CORE_SIZE;
         break;
      case INDIR_B:
         return (*curr_pc + rel_addr + core[*curr_pc+rel_addr].arg2) % CORE_SIZE;
         break;
/* Leave these alone till I can figure out how the fuck they're supposed to work
      case PREDEC_A:
         return (pc + rel_addr + core[pc+rel_addr].arg1)-- % CORE_SIZE;
         break;
      case PREDEC_B:
         return (pc + rel_addr + core[pc+rel_addr].arg2)-- % CORE_SIZE;
         break;
      case POSTINC_A:
         return (pc + rel_addr + core[pc+rel_addr].arg1)++ % CORE_SIZE;
         break;
      case POSTINC_B:
         return (pc + rel_addr + core[pc+rel_addr].arg2)++ % CORE_SIZE;
         break;
*/
   }
   return *curr_pc;
}

void addInstrs(){
   InstrNode *tmp = nodes;
   int tmpPC = pc[0];
   while( tmp != NULL ){
      core[tmpPC].opcode = tmp->opcode;
      core[tmpPC].arg1 = tmp->arg1;
      core[tmpPC].arg2 = tmp->arg2;

      core[tmpPC].modifier = tmp->modifier;
      core[tmpPC].mode_a = tmp->mode_a;
      core[tmpPC].mode_b = tmp->mode_b;

      tmp = tmp->next;
      tmpPC++;
   }
}

void print_instr(CoreCell instr){
   char *command;
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
         arg1 = (*curr_pc + instr.arg1) % 8000 - *curr_pc;
         arg2 = -1;
         break;
      default:
         command = "HCF";
         break;
   }
   printf("%4d: %s %c%d",  *curr_pc, command, get_addr_char(instr.mode_a), arg1);
   if (arg2 > 0)
      printf(", %c%d", get_addr_char(instr.mode_b), arg2);
   printf("\n");
}

char get_addr_char(int addr_mode){
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
