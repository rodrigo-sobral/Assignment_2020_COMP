
TESTS_PATH= ./tests/our_tests/
OUTPUTS_PATH= ${TESTS_PATH}outputs/

RUNNING_FILE_NAME= comments
RUNNING_FILE= ${RUNNING_FILE_NAME}.uc
OUTPUT_FILE= ${RUNNING_FILE_NAME}.out


run:
	lex ./uccompiler.l
	clang-3.9 -o ./uccompiler lex.yy.c
	./uccompiler -l  <${TESTS_PATH}${RUNNING_FILE}> ${OUTPUTS_PATH}${OUTPUT_FILE}

clear:
	rm ./uccompiler
	rm ./lex.yy.c
	rm ${OUTPUTS_PATH}*