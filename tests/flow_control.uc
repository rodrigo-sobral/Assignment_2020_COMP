/* Neste teste analizamos
    os
    *controlos de fluxo* dos programas      aqui
// inseridos
*/



short main      (void) {
    chr letra_abc_1= 'a';

    whil (1) {
        while (letra_abc_1 < 'z') {
            if (letra_abc_1=='j') break ;
            letra_abc_1++;
        }

    return 0;
}
    

void teste(void) {
    int a_b=0;
    short lim3=50;
    
    while ( a_b < lim3 )
        a_b++;

    if (a_b==50) { return 50; }
    else return 0
}