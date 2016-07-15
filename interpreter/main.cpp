#include <iostream>
#include <iomanip>
#include <string>
#include "interpreter.hpp"
#include "red.tab.h"
#define CORE_SIZE 8000
#define MAX_CYCLES 8000
#define MAX_PROCESSES 8000

Core *interp;
extern std::vector<InstrNode> nodes;

int main(int argc, char **argv){
   yyparse();

   interp = new Core(CORE_SIZE, MAX_CYCLES, MAX_PROCESSES);
   
   interp->process_idx = 0;
   interp->num_processes = 1;
   interp->addInstrs(nodes);
   interp->run();
}
