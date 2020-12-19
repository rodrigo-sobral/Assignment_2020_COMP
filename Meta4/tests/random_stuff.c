int main(void) {
    int i=0;
    putchar(i);
    i=20;
    putchar(i);

    char a='A';
    while (a<'Z') {
        putchar(a);
        a=a+1;
    }
    
    putchar('\n');
    short b='\n';
    while (i<'Z') {
        putchar(i);
        i=i+1;
    }

    putchar(':');
    short f= getchar();
    putchar(f+1);

    double t= 5.0;
    double resultado= 5*10.0;

    if (t==42 && 4==3 || 4&3==5|2) { }
    double soma = t+2;
    double sub= t-1;
    double mult= t*3;
    double div= t/2;
    int res1= (1*1)/1+1-1*(1+1);

    putchar(res1);
    if (!(1==1)) {
        putchar(res1);
    }


    putchar('\n');
    return 0;
}
 