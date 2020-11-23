
TESTS_PATH= ./tests/
OUTPUTS_PATH= ${TESTS_PATH}outputs/

RUNNING_PROG= uccompiler
RUNNING_FILE_NAME= miscellaneous_test
RUNNING_FILE= ${RUNNING_FILE_NAME}.uc
OUTPUT_FILE= ${RUNNING_FILE_NAME}.out


run:
	lex ./${RUNNING_PROG}.l
	yacc -d -v ${RUNNING_PROG}.y
	gcc -o ${RUNNING_PROG} y.tab.c lex.yy.c ast.c
	cc -g -o uccompiler lex.yy.c y.tab.c semantics.c ast.c symbol_table.c
	./${RUNNING_PROG} -t < ${TESTS_PATH}${RUNNING_FILE} > ${OUTPUTS_PATH}${OUTPUT_FILE}

clear:
	rm ./${RUNNING_PROG}
	rm ./lex.yy.c
	rm y.output
	rm y.tab.c
	rm y.tab.h
	rm ${OUTPUTS_PATH}*
