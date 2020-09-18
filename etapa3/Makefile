CC=gcc
LEX=flex
YACC=yacc

FILENAME=etapa3

${FILENAME}: lex.yy.o hash.o main.o y.tab.o ast.o decompiler.o
	${CC} -o ${FILENAME} lex.yy.o hash.o y.tab.o ast.o decompiler.o
main.o: main.c
	${CC} -c main.c
decompiler.o: decompiler.c
	${CC} -c decompiler.c

hash.o: hash.c
	${CC} -c hash.c
ast.o: ast.c
	${CC} -c ast.c
	

lex.yy.o: lex.yy.c y.tab.o
	${CC} -c lex.yy.c
y.tab.o: y.tab.c
	${CC} -c y.tab.c

lex.yy.c: scanner.l
	${LEX} scanner.l 
y.tab.c: parser.y
	${YACC} parser.y -d

test: ${FILENAME} test/input.test
	./${FILENAME} test/input.test test/output1.test
	./${FILENAME} test/output1.test test/output2.test 
	diff test/output1.test test/output2.test

clean:
	rm *.o lex.yy.c y.tab.* ${FILENAME} ${FILENAME}.tgz test/output*.test