programa : analizador_lexico/lexico.l analizador_sintactico/parser.y 
	make -C analizador_lexico/
	make -C analizador_sintactico/ 
	mv analizador_lexico/lex.yy.c build/
	mv analizador_sintactico/parser.tab.h build/
	mv analizador_sintactico/parser.tab.c build/
	gcc build/parser.tab.c build/lex.yy.c -o build/main

test : build/main
	./build/main tests/test8.txt	
