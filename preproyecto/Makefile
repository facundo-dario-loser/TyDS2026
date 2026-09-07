PROGRAMAS_C = analizador_sintactico/parser.tab.c 		\
			  analizador_lexico/lex.yy.c 		  		\
			  TADs/ts.c 						  		\
			  TADs/ast.c 								\
			  analizador_semantico/analisis_semantico.c \
			  interprete/interprete.c 				    \
			  pseudo_asm/ir_asm.c 					    \
			  main.c

main : analizador_lexico/lexico.l analizador_sintactico/parser.y $(PROGRAMAS_C)
	make -C analizador_lexico/
	make -C analizador_sintactico/ 
	gcc $(PROGRAMAS_C) -o build/preproyecto

test : build/preproyecto
	./build/preproyecto -p tests/test9_error.txt	
