# TyDS2026
repositorio de la materia Taller de Diseño de Software UNRC 2026

### Integrante: Loser Facundo Dario

## Instrucciones para compilar

si se cuenta con la herramienta `Make` instalada ejecutar en la terminal:

```
cd preproyecto
make
```

en caso de no tener `Make` entonces hacer:

```
cd preproyecto/analizador_lexico
flex lexico.l 
cd ..

cd analizador_sintactico
bison parser.y 
cd ..

gcc analizador_sintactico/parser.tab.c 		  \
	analizador_lexico/lex.yy.c 		  		  \
	TADs/ts.c 						  		  \
	TADs/ast.c 								  \
	analizador_semantico/analisis_semantico.c \
	interprete/interprete.c 				  \
	pseudo_asm/ir_asm.c 					  \
	main.c                                    \
    -o build/preproyecto
```

luego de esto se creara el ejecutable llamado `preproyecto` dentro de la carpeta `/preproyecto/build`

## Uso: 

para usar el programa se debe proveer primero un flag para indicar si se desea ejecutar el interprete o se quiere generar pseudo assembly y luego se debe indicar el path al archivo con el programa fuente

**Flags:**
- `-i` (intérprete)
- `-p` (generar pseudo assembly)

```
./build/preproyecto -flag path_to_source_code
```
**Aclaración:**

El interprete la unica salida que genera es un print del valor resultante de evaluar la expresion asociada a un return (`return exp;`). Si no se retorna nada, el interprete no genera ninguna salida.

En el caso del generador de pseudo assembly al finalizar imprime en la terminal todas las instrucciones generadas.

## Organizacion

dentro de `/preproyecto` se cuenta con las siguientes carpetas y archivos:

`/analizador_lexico`: contiene el lexer hecho con flex

`/analizador_sintactico`: contiene el parser hecho con bison

`/analizador_semantico`: codigo correspondiente al analisis semantico

`/interprete`: codigo correspondiente al interprete

`/pseudo_asm`: codigo correspondiente a la generacion de pseudo assembly

`/TADs`: contiene la implementacion del arbol sintactico abstracto (`ast.h/.c`) y de la tabla de simbolos (`ts.h/.c`)

`/build`: aca se guarda el ejecutable final llamado `preproyecto`

`/tests`: algunos tests con los que se probo el proyecto

`main.c`: punto de entrada de todo el programa


## Gramatica del lenguaje
**Aclaracion**: esto no es codigo de bison. Esta escrita asi para que sea mas legible
```
type -> INT | BOOL | VOID

p    -> type MAIN ( ) { c }

c    -> d c | s c | λ		  // cuerpo de la funcion main

e    -> e + e                 // expresion
e    -> e * e
e    -> e AND e 
e    -> e OR e
e    -> ( e )
e    -> CONSTANTE_NUMERICA
e    -> CONSTANTE_BOOLEANA 
e    -> ID

s    -> ID = e ; 			  // sentencia 
s    -> RETURN e ;       
s    -> RETURN ;          

d    -> type ID ;             // declaracion
```