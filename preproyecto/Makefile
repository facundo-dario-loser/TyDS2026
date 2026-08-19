programa: lexico.l # make programa
	flex lexico.l 
	gcc lex.yy.c -o lexico 

test: programa 	   # make test 
	./lexico tests/test1.txt

