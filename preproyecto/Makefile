programa : analizador_lexico/lexico.l analizador_sintactico/parser.y 
	make -C analizador_lexico/
	make -C analizador_sintactico/ 
	mv analizador_lexico/lex.yy.c build/
	mv analizador_sintactico/parser.tab.h build/
	mv analizador_sintactico/parser.tab.c build/
	cp analizador_semantico/analisis_semantico.h build/
	cp analizador_semantico/analisis_semantico.c build/
	cp TADs/ast.h build/ast.h
	cp TADs/ast.c build/ast.c
	cp TADs/ts.h build/ts.h
	cp TADs/ts.c build/ts.c
	cp interprete/interprete.h build/
	cp interprete/interprete.c build/
	cp pseudo_asm/ir_asm.h build/
	cp pseudo_asm/ir_asm.c build/
	gcc build/parser.tab.c build/lex.yy.c build/ts.c build/ast.c build/analisis_semantico.c build/interprete.c build/ir_asm.c main.c -o build/main

test : build/main
	./build/main tests/test9_error.txt	
