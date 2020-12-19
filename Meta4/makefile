
TESTS_PATH= ./tests/
RUNNING_PROG= uccompiler

orRUNNING_FILE_NAME= hello
RUNNING_FILE= ${TESTS_PATH}${RUNNING_FILE_NAME}.c

OUTPUTS_PATH= ${TESTS_PATH}outputs/
OUTPUT_FILE= ${OUTPUTS_PATH}${RUNNING_FILE_NAME}.ll

run:
	lex ./${RUNNING_PROG}.l
	yacc -d -v ${RUNNING_PROG}.y
	cc -g -Wall -o ${RUNNING_PROG} lex.yy.c y.tab.c semantics.c ast.c symbol_table.c code_generator.c
	./${RUNNING_PROG} < ${RUNNING_FILE} > ${OUTPUT_FILE}
	lli ${OUTPUT_FILE}

clear:
	rm ./${RUNNING_PROG}
	rm ./lex.yy.c
	rm y.output
	rm y.tab.c
	rm y.tab.h
	rm ${OUTPUTS_PATH}*

