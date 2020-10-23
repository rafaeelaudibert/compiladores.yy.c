CC=gcc
LEX=flex
YACC=yacc

FILENAME=etapa4

${FILENAME}: lex.yy.o hash.o main.o y.tab.o ast.o dataType.o semanticParser.o chainedList.o typeInfer.o
	${CC} -o ${FILENAME} lex.yy.o hash.o y.tab.o ast.o dataType.o semanticParser.o chainedList.o typeInfer.o
main.o: main.c
	${CC} -c main.c

semanticParser.o: semanticParser.c
	${CC} -c semanticParser.c
typeInfer.o: typeInfer.c
	${CC} -c typeInfer.c

dataType.o: dataType.c
	${CC} -c dataType.c

hash.o: hash.c
	${CC} -c hash.c
ast.o: ast.c
	${CC} -c ast.c
chainedList.o: chainedList.c
	${CC} -c chainedList.c
	

lex.yy.o: lex.yy.c y.tab.o
	${CC} -c lex.yy.c
y.tab.o: y.tab.c
	${CC} -c y.tab.c

lex.yy.c: scanner.l
	${LEX} scanner.l 
y.tab.c: parser.y
	${YACC} parser.y -d

test: ${FILENAME} test/input_correct.test test/input_incorrect.test
	./${FILENAME} test/input_correct.test > test/output.test
	- ./${FILENAME} test/input_incorrect.test > test/output.test
	diff test/expected.test test/output.test

clean:
	- rm *.o lex.yy.c y.tab.* ${FILENAME} ${FILENAME}.tgz test/output*.test