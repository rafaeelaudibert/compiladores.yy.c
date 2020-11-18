CC=gcc
LEX=flex
YACC=yacc
FLAGS=-Wall -O2 -g

FILENAME=etapa6

${FILENAME}: lex.yy.o hash.o main.o y.tab.o ast.o dataType.o semanticParser.o chainedList.o typeInfer.o tac.o codegen.o asmGeneration.o
	${CC} ${FLAGS} -o ${FILENAME} lex.yy.o hash.o y.tab.o ast.o dataType.o semanticParser.o chainedList.o typeInfer.o tac.o codegen.o asmGeneration.o
main.o: main.c
	${CC} ${FLAGS} -c main.c

semanticParser.o: semanticParser.c
	${CC} ${FLAGS} -c semanticParser.c
typeInfer.o: typeInfer.c
	${CC} ${FLAGS} -c typeInfer.c

codegen.o: codegen.c
	${CC} ${FLAGS} -c codegen.c
asmGeneration.o: asmGeneration.c
	${CC} ${FLAGS} -c asmGeneration.c

dataType.o: dataType.c
	${CC} ${FLAGS} -c dataType.c

hash.o: hash.c
	${CC} ${FLAGS} -c hash.c
ast.o: ast.c
	${CC} ${FLAGS} -c ast.c
chainedList.o: chainedList.c
	${CC} ${FLAGS} -c chainedList.c
tac.o: tac.c
	${CC} ${FLAGS} -c tac.c
	

lex.yy.o: lex.yy.c y.tab.o
	${CC} -c lex.yy.c
y.tab.o: y.tab.c
	${CC} -c y.tab.c

lex.yy.c: scanner.l
	${LEX} scanner.l
y.tab.c: parser.y
	${YACC} parser.y -d

test: ${FILENAME} test/input.test test/expected.test
	./${FILENAME} test/input.test
	gcc out.s -o test/output.exe
	- ./test/output.exe > test/output.test
	diff test/output.test test/expected.test

test_all:  ${FILENAME} test/input.test test/expected.test
	./${FILENAME} test/input.test
	gcc out.s -o test/output.exe
	- ./test/output.exe > test/output.test
	diff test/output.test test/expected.test
	echo "Teste básico ✔️"
	./${FILENAME} test/fatorial_input.test
	gcc out.s -o test/output.exe
	- ./test/output.exe > test/output.test
	diff test/output.test test/fatorial_expected.test
	echo "Teste fatorial ✔️"
	./${FILENAME} test/iterative_fib_input.test
	gcc out.s -o test/output.exe
	- ./test/output.exe > test/output.test
	diff test/output.test test/iterative_fib_expected.test
	echo "Teste fibonacci iterativo ✔️"
	./${FILENAME} test/recursive_fib_input.test
	gcc out.s -o test/output.exe
	- ./test/output.exe > test/output.test
	diff test/output.test test/recursive_fib_expected.test
	echo "Teste fibonacci recursivo ✔️"

clean:
	- rm *.o lex.yy.c y.tab.* ${FILENAME} ${FILENAME}.tgz test/output.* *.s