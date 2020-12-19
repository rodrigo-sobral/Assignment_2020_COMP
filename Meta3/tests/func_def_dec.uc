//Neste código: 
void f20(char x, char y, int z){
	void p;
	int p;
}
//devem encontrar apenas o erro Invalid use of void type in declaration

void f1(void){}

char f1(void) {}


void d(void);
int main(void) {
    int a, b, c;
    if(main) {b=1; c=2;} else {c=1;}
    if (d){}
}
//Deve dar estes erros:
//Line 12, col 8: Conflicting types (got int(void), expected int)
//4c3
//Line 16, col 9: Conflicting types (got void(void), expected int)


int B(int a){
	int x;
	x=2;
	a=3;
}

int B(int);


int C(int);
int C(int);

int C(int a){}


int D(int);
int D(int);

int D(int a){}
int D(int a){}



int E(int);

int E(int a){}
int E(double b){}
double E(int a){}


int F(double);
int F(int a){}



int G(short a);
short G(int){
	int y, v;
	return v;
}

int H(short a);
int H(short a, short b);


int I(int,int);
int I(int);

int J(short a) {
	double d, b;
	return b;
}
int J(short a, short b){
	int a, b;
	return b;
}


int K(short a, short b){
	int o, p;
	return p;
}
int K(short a){
	int t, r;
	return t;
}