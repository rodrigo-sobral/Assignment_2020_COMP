int i = (1&&23);
char a1='a'+3, a2='b'+2, a3='c'+4; short a4=3;
int j; int j1=(j=1);

void printarHello(void){
	char a='H';
	char b = 'e';
	putchar(a);
   	putchar(b);
	putchar('l');
	putchar('l');
	putchar('o');
	char c =' ';
	char d ='W';
	putchar(c);
	putchar(d);
	char e ='o';
	putchar(e);
	putchar('r');
	putchar('l');
	putchar('d');
	char f='\n';
	putchar(f);
}
void imprimeAbecdario(void) {
    char a='A';
    while (a<'z') {
        putchar(a);
        a=a+1;
    }
}

int somarNumeros(int a, short b, int c) { return a+b+c; }
int subtrairNumeros(char a, short b) { 
    int a4_aux= a4;
    short a5= a-(a4_aux+b);
    return a5;
}
short retornaInversoASCII(short negativo) { return -negativo; }

char recebeLetraSeguinte(char letra) {
    putchar(':');
    return getchar()+1;
}

int main(void) {
    double t= 5.0;
    double resultado= 5*10.0;

    if (t==42 && 4==3 || 4&3==5|2) { }

    int ch=32;
    while (ch<=128 && ch!=0 && ch!=80) {
        if (ch%2==0) putchar(ch);
        else putchar(' ');
        ch= ch+1;
    }
    putchar('\n');
    printarHello();
    putchar('\n');
    imprimeAbecdario();
    putchar('\n');
    putchar(recebeLetraSeguinte('1'));
    putchar('\n');
    putchar(somarNumeros(49+1,1*1,2-1));
    putchar('\n');
    putchar(subtrairNumeros('9', 3));
    putchar('\n');
    putchar(retornaInversoASCII(-50));
    putchar('\n');
    putchar(somarNumeros(50,j,j1));

    putchar('\n');
    return 0;
}
 