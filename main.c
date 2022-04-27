#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gravacomp.h"

struct teste1 {
    int  i1;
    char s1[8];
    char s2[5];
    unsigned int u1;
}Teste1[2] = {{-30,"awd","x",999999},{-300,"DKIfd","2134",43}};

int main(void){
    FILE * arquivo;
    arquivo = fopen("arquivo.bin","wb");
    gravacomp(2, &Teste1, "is08s05u",arquivo);
    fclose(arquivo);
}