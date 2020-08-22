CC=gcc
LEX=flex
FILENAME=etapa1

${FILENAME}: lex.yy.o hash.o main.o
	${CC} -o ${FILENAME} lex.yy.o hash.o
main.o: main.c
	${CC} -c main.c
hash.o: hash.c
	${CC} -c hash.c
lex.yy.o: lex.yy.c
	${CC} -c lex.yy.c
lex.yy.c: scanner.l
	${LEX} scanner.l 

test: ${FILENAME} test/input.test test/expected.test
	./${FILENAME} < test/input.test > test/output.test && diff test/output.test test/expected.test

clean:
	rm *.o lex.yy.c ${FILENAME} ${FILENAME}.tgz