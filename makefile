
TESTS_PATH= ./tests/
OUTPUTS_PATH= ${TESTS_PATH}outputs/

RUNNING_FILE_NAME= comments
RUNNING_FILE= ${RUNNING_FILE_NAME}.uc
OUTPUT_FILE= ${RUNNING_FILE_NAME}.out


run:
	lex ./code/uccompiler.l
	clang-3.9 -o ./code/uccompiler lex.yy.c
	./code/uccompiler -l  <${TESTS_PATH}${RUNNING_FILE}> ${OUTPUTS_PATH}${OUTPUT_FILE}

clear:
	rm ./code/uccompiler
	rm ./lex.yy.c
	rm ${OUTPUTS_PATH}*