red : red.l red.y
	bison -dvt red.y
	flex -o red.lex.c red.l
	gcc -Wall -std=gnu99 -g -o test red.tab.c red.lex.c main.c


.PHONY : clean
clean:
	/bin/rm -f test red.lex.c red.tab.c red.tab.h 
