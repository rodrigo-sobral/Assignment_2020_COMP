//2327: Neste código não estão a tratar o erro dos parâmetros repetidos:

double f7(short a, double b, int b);
//SYMALREADY DEFINED

//Também não estão a detectar todas as redefinições das funções. Ex:
char f1(void){}
double f2(double a){}
void f3(char a){}

int f2(int a, int b){}
void f3(char a, char b){}
int f2(int a, int p){}

//Também não estão a detectar o erro nestas operações:
int t = 1 + 2.0;
char h = 15.0;
int d = .04e23 + 2;


//2328:

int f(void) {}
int main(void) {
    f + main;
}
//o erro em f + main é o de Operator + cannot be applied to types int(void), int(void)
//MEsma coisa para operações de assignaçã