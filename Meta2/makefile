
TESTS_PATH= ./tests/
OUTPUTS_PATH= ${TESTS_PATH}outputs/

RUNNING_FILE_NAME= exp_test
RUNNING_FILE= ${RUNNING_FILE_NAME}.uc
OUTPUT_FILE= ${RUNNING_FILE_NAME}.out


run:
	lex ./uccompiler.l
	yacc -d -v uccompiler.y
	gcc -o uccompiler y.tab.c lex.yy.c ast.c
	./uccompiler < ${TESTS_PATH}${RUNNING_FILE} > ${OUTPUTS_PATH}${OUTPUT_FILE}

clear:
	rm ./uccompiler
	rm ./lex.yy.c
	rm y.output
	rm y.tab.c
	rm y.tab.h
	rm ${OUTPUTS_PATH}*
