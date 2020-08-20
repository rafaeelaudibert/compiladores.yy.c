CC=gcc
LEX=$(shell if ! type "lex" &> /dev/null; then echo "flex"; else echo "lex"; fi)
FILENAME=etapa1

${FILENAME}: lex.yy.c
	${CC} -o ${FILENAME} lex.yy.c
lex.yy.c: scanner.l
	${LEX} scanner.l

clean:
	rm lex.yy.c ${FILENAME} ${FILENAME}.tgz