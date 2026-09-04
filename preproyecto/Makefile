main : analizador_lexico/lexico.l analizador_sintactico/parser.y 
	make -C analizador_lexico/
	make -C analizador_sintactico/ 

	gcc analizador_sintactico/parser.tab.c   	  \
		analizador_lexico/lex.yy.c			      \
		TADs/ts.c 								  \
		TADs/ast.c 								  \
		analizador_semantico/analisis_semantico.c \
		interprete/interprete.c 				  \
		pseudo_asm/ir_asm.c 					  \
		main.c 									  \
		-o build/main

test : build/main
	./build/main tests/test9_error.txt	
