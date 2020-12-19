void main(void) {
    int i=0;
    putchar(i);
    i=20;
    putchar(i);

    short a='\n';
    while (a<'Z') {
        putchar(a);
        a=a+1;
    }
/*
    putchar('\n');
    short i='\n';
    while (i<'Z') {
        putchar(i);
        i=i+1;
    }
    */
    putchar(':');
    short soma= getchar();
    putchar(soma+1);

    double resultado= 5*10;

    putchar('\n');
}
