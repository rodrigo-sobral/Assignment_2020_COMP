int func(int i);
int main(void){
	char i = 'A';

  	while(i>1&&i!=30) i=i-4;
  
	while(i>1||i!=30){
		if (i=='B') return 0;
		i=i-4;
		while((i=0)||(i=1)&&1*i+9%2){
			i=i+10;
			putchar(i);
			if(putchar('\n')==0){
				//i=1-5*1*1*1*1/1/1%2*2;
				//i=45&&4||2^3&&1;
			}
			else putchar('o');
			while(func(i)) i=i+1;
		}
	}
	return 1;
	putchar('*');
	return 0;
}

int func(int i) { return func(i+1)*1*2; }