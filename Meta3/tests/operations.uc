
void funcaoQueRecebeInteiro(int inteiro) {
    return ;
}

int main(void) {
    int a=5, b=4;
    short c=3;
    double d=12.5;
    char e= 'a';
    
    funcaoQueRecebeInteiro(a/b);
    funcaoQueRecebeInteiro(a*c);
    funcaoQueRecebeInteiro(c-b);
    funcaoQueRecebeInteiro(c-b);
    funcaoQueRecebeInteiro(d*b);
    funcaoQueRecebeInteiro(d%c);
    funcaoQueRecebeInteiro(e*a);
    funcaoQueRecebeInteiro(e*d);
    funcaoQueRecebeInteiro(e*funcaoQueRecebeInteiro(5));
    funcaoQueRecebeInteiro(a*funcaoQueRecebeInteiro(5));
    
    return 1;
}