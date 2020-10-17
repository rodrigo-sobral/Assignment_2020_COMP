/*Fahrenheit-Celsius table; floating-point version. */

int main(void)
{
  float fahr, celsius;
  int lower, upper, step; //
  lower = 0;
  upper = 300;  
  step = 20;
  fahr = lower;
  while(fahr <= upper)
    {
      celsius = (5.0/9.0) * (fahr-32); ///
      fahr += step; /////////
    }
  return 0;
}
