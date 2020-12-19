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
   } else if(n == 1) {
      return 1;
   } else {
      return (fibbonacci(n-1) + fibbonacci(n-2));
   }
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
}