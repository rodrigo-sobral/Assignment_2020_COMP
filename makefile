run:
	lex code/uccompiler.l
	clang-3.9 -o code/uccompiler lex.yy.c
	./code/uccompiler -l