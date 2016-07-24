CFLAGS=-Wall -g -std=c++11 -I interpreter

red : interpreter/red.l interpreter/red.y
	bison -dvt interpreter/red.ypp -o interpreter/red.tab.cpp
	flex -o interpreter/red.lex.c interpreter/red.l
	g++ ${CFLAGS} -o test interpreter/red.tab.cpp interpreter/red.lex.c interpreter/interpreter.cpp interpreter/main.cpp


.PHONY : clean
clean:
	/bin/rm -f test interpreter/red.lex.c interpreter/red.tab.c interpreter/red.tab.h interpreter/*.tab.* interpreter/*.lex.*
