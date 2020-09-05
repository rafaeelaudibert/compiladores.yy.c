CC=gcc
LEX=flex
YACC=yacc

FILENAME=etapa2

${FILENAME}: lex.yy.o hash.o main.o y.tab.o
	${CC} -o ${FILENAME} lex.yy.o hash.o y.tab.o
main.o: main.c
	${CC} -c main.c

hash.o: hash.c
	${CC} -c hash.c

lex.yy.o: lex.yy.c y.tab.o
	${CC} -c lex.yy.c
y.tab.o: y.tab.c
	${CC} -c y.tab.c

lex.yy.c: scanner.l
	${LEX} scanner.l 
y.tab.c: parser.y
	${YACC} parser.y -d

test: ${FILENAME} test/input.test
	./${FILENAME} test/input.test

clean:
	rm *.o lex.yy.c y.tab.* ${FILENAME} ${FILENAME}.tgz