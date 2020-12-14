//  ERROS DE E


//  deve dar erro de parametros repetidos
double f7(short a, double b, int b);

//  deve dar erro de funçao ja definida
int f2(int a, int b){}
void f3(char a, char b){}
int f2(int a, int p){}


//  ERROS DE F

//  o erro em f + main é o de Operator + cannot be applied to types int(void), int(void)
//  MEsma coisa para operações de assignação

int t = 1 + 2.0;
char h = 15.0;
int d = .04e23 + 2;

int f(void) {}
int main(void) {
    f + main;
}