int factorial(int n) {
   //base case
   if(n == 0) {
      return 1;
   } else {
      return n * factorial(n-1);
   }
}

int fibbonacci(int n) {
   if(n == 0){
      return 0;
   } if(n == 1) {
      return 1;
   } else {
      return (fibbonacci(n-1) + fibbonacci(n-2));
   }
}

int gcd(int a, int b) {
   if (a == 0) return b;
   if (b == 0)return a; 
   if (a == b) return a;
   if (a > b) return gcd(a-b, b);

   return gcd(a, b-a);
}
 
int main(void) {
   int n = 5;
   int i=0;
   putchar('\n');
   putchar('0'+fibbonacci(i));
    
   while(i<n) {
      i=i+1;
      putchar('-');
      putchar(fibbonacci(i)+'0');
   }
   putchar('\n');
   
   int j, a='A';
   while(i<20&&(j=factorial(i))&&(i=i+1)) {
      putchar(a);
      a=a+1;
   }
   putchar('\n');
}