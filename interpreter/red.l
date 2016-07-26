%{
extern int yychar;
extern int errcode;
void yyerror(char *s);
#include "red.tab.hpp"
%}

%option noyywrap
%option nounput
%option noinput
%option yylineno
/* Declarations */

DIGIT [0-9]
LETTER [a-zA-Z]
ALPHANUM [0-9a-zA-Z]

%s COMMENT

%%
 /* Rules */

";"                  {BEGIN( COMMENT ); yylval.ch=yytext[0]; return yytext[0];}
<COMMENT>[^\n]*      {yylval.charptr=strdup(yytext); return STRING;}
<COMMENT>\n          {BEGIN( INITIAL );}

-?{DIGIT}{DIGIT}*      {yylval.intcon=atoi(yytext);    return INTCON;}
"DAT"                {yylval.charptr=strdup(yytext); return OPDAT;}
"MOV"                {yylval.charptr=strdup(yytext); return OPMOV;}
"ADD"                {yylval.charptr=strdup(yytext); return OPADD;}
"SUB"                {yylval.charptr=strdup(yytext); return OPSUB;}
"MUL"                {yylval.charptr=strdup(yytext); return OPMUL;}
"DIV"                {yylval.charptr=strdup(yytext); return OPDIV;}
"MOD"                {yylval.charptr=strdup(yytext); return OPMOD;}
"JMP"                {yylval.charptr=strdup(yytext); return OPJMP;}
"JMZ"                {yylval.charptr=strdup(yytext); return OPJMZ;}
"JMN"                {yylval.charptr=strdup(yytext); return OPJMN;}
"DJN"                {yylval.charptr=strdup(yytext); return OPDJN;}
"SPL"                {yylval.charptr=strdup(yytext); return OPSPL;}
"CMP"                {yylval.charptr=strdup(yytext); return OPCMP;}
"SEQ"                {yylval.charptr=strdup(yytext); return OPSEQ;}
"SNE"                {yylval.charptr=strdup(yytext); return OPSNE;}
"SLT"                {yylval.charptr=strdup(yytext); return OPSLT;}
"LDP"                {yylval.charptr=strdup(yytext); return OPLDP;}
"STP"                {yylval.charptr=strdup(yytext); return OPSTP;}
"NOP"                {yylval.charptr=strdup(yytext); return OPNOP;}
".A"                 {yylval.charptr=strdup(yytext); return MOD_A;}
".B"                 {yylval.charptr=strdup(yytext); return MOD_B;}
".AB"                {yylval.charptr=strdup(yytext); return MOD_AB;}
".BA"                {yylval.charptr=strdup(yytext); return MOD_BA;}
".F"                 {yylval.charptr=strdup(yytext); return MOD_F;}
".X"                 {yylval.charptr=strdup(yytext); return MOD_X;}
".I"                 {yylval.charptr=strdup(yytext); return MOD_I;}
","                  {yylval.ch=yytext[0]; return yytext[0];}
[#$*@{}<>]           {yylval.ch=yytext[0]; return yytext[0];}
{LETTER}{ALPHANUM}*  {yylval.charptr=strdup(yytext); return LABEL;}
[[:space:]]
.                    {yylval.ch=yytext[0];return UNKNOWN;}

%%
