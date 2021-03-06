
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
extern int yychar, yydebug, yylineno;
extern char *yytext;
extern InstrNode *nodes;
extern InstrNode *lastNode;
int yylex();
void yyerror(char *s);

InstrNode *addInstr(int opcode, int modifier, int mode_a, int arg1, int mode_b, int arg2);
%}

%union{
   char *charptr;
   char ch;
   int intcon;

   int opcode;
   int addr_mode;
   int modifier;
}

/* tokens */

%token <charptr>  OPDAT OPMOV OPADD OPSUB OPMUL OPDIV OPMOD OPJMP OPJMZ OPDJN OPSPL OPCMP OPSEQ OPSNE OPSLT OPLDP OPSTP OPNOP 
%token <charptr>  MOD_A MOD_B MOD_AB MOD_BA MOD_F MOD_X MOD_I
%token <charptr>  STRING
%token <ch>       ','
%token <ch>       UNKNOWN
%token <intcon>   INTCON

%type <opcode>       opcode
%type <addr_mode>    addr_mode
%type <modifier>    modifier

%%

stmt_list : stmt stmt_list
          | /* epsilon */
          ;

stmt : opcode modifier addr_mode INTCON ',' addr_mode INTCON {addInstr($1, $2, $3, $4, $6, $7);}
     | opcode modifier addr_mode INTCON {addInstr($1, $2, $3, $4, 0, 0);}
     | opcode modifier error INTCON ',' error INTCON
     | comment
     ;

comment : ';' STRING
        ;

opcode : OPDAT {$$ = DAT;}
       | OPMOV {$$ = MOV;}
       | OPADD {$$ = ADD;}
       | OPSUB {$$ = SUB;}
       | OPMUL {$$ = MUL;}
       | OPDIV {$$ = DIV;}
       | OPMOD {$$ = MOD;}
       | OPJMP {$$ = JMP;}
       | OPJMZ {$$ = JMZ;}
       | OPDJN {$$ = DJN;}
       | OPSPL {$$ = SPL;}
       | OPCMP {$$ = CMP;}
       | OPSEQ {$$ = SEQ;}
       | OPSNE {$$ = SNE;}
       | OPSLT {$$ = SLT;}
       | OPLDP {$$ = LDP;}
       | OPSTP {$$ = STP;}
       | OPNOP {$$ = NOP;}
       | error {$$ = DAT;}
       ;

addr_mode : '#' {$$ = IMM;}
          | '$' {$$ = DIRECT;}
          | '*' {$$ = INDIR_A;}
          | '@' {$$ = INDIR_B;}
          | '{' {$$ = PREDEC_A;}
          | '}' {$$ = POSTINC_A;}
          | '<' {$$ = PREDEC_B;}
          | '>' {$$ = POSTINC_B;}
          | /* epsilon */ {$$ = DIRECT;}
          ;

modifier : MOD_A     {$$ = A;}
         | MOD_B     {$$ = B;}
         | MOD_AB    {$$ = AB;}
         | MOD_BA    {$$ = BA;}
         | MOD_F     {$$ = F;}
         | MOD_X     {$$ = X;}
         | MOD_I     {$$ = I;}
         | /* epsilon */ {$$ = NONE;}
         ;
%%
//int main(int argc, char **argv){
//   nodes = malloc(sizeof(InstrNode));
//   nodes->next = NULL;
//   lastNode = nodes;
//   yyparse();
//}

InstrNode *addInstr(int opcode, int modifier, int mode_a, int arg1, int mode_b, int arg2){
   InstrNode *node = (InstrNode *) malloc(sizeof(InstrNode));
   node->opcode = opcode;
   node->arg1 = arg1 % CORE_SIZE;
   node->arg2 = arg2 % CORE_SIZE;
   node->mode_a = mode_a;
   node->mode_b = mode_b;
   node->modifier = modifier;

   if( node->arg1 < 0 ){ node->arg1 += CORE_SIZE; }
   if( node->arg2 < 0 ){ node->arg2 += CORE_SIZE; }
   if( nodes == NULL){
      nodes = node;
      lastNode = node;
   }
   else{
      lastNode->next = node;
      lastNode = node;
   }
   return node;
}

void print_tok(){
   if (yytext[0] == 0){
      fprintf(stderr, "EOF");
   } else if (yychar < 255){
      fprintf(stderr, "\"%c\"", yychar);
   } else {
      switch (yychar){
      case INTCON:
         fprintf(stderr,"\"%d\"", yylval.intcon);
         break;
      case '#':
      case '$':
      case '*':
      case '@':
      case '{':
      case '}':
         fprintf(stderr,"\"%c\"", yylval.ch);
         break;
      case OPDAT:
      case OPMOV:
      case OPADD:
      case OPSUB:
      case OPMUL:
      case OPDIV:
      case OPMOD:
      case OPJMP:
      case OPJMZ:
      case OPDJN:
      case OPSPL:
      case OPCMP:
      case OPSEQ:
      case OPSNE:
      case OPSLT:
      case OPLDP:
      case OPSTP:
      case OPNOP:
      case MOD_A:
      case MOD_B:
      case MOD_AB:
      case MOD_BA:
      case MOD_F:
      case MOD_X:
      case MOD_I:
         fprintf(stderr,"\"%s\"", yylval.charptr);
         break;
      default:
         fprintf(stderr,"\"%c\"", yylval.ch);
         break;
      }
   }
}

void yyerror(char *s){
   fprintf(stderr, "Line %d: %s: unexpected ", yylineno, s); 
   print_tok();
   fprintf(stderr,"\n");
}

