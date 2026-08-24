programa : analizador_lexico/lexico.l analizador_sintactico/parser.y 
	make -C analizador_lexico/
	make -C analizador_sintactico/ 
	cp analizador_lexico/lex.yy.c analizador_sintactico/
	cp analizador_sintactico/parser.tab.h analizador_lexico
	gcc analizador_sintactico/parser.tab.c analizador_lexico/lex.yy.c -o main

test : main
	./main tests/test1.txt	
