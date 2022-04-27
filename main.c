#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gravacomp.h"

struct teste1 {
    int  i1;
    char s1[5];
    //char s2[5];
    unsigned int u1;
}Teste1[2] = {{-1,"abc",258},{1,"ABCD",65535}};

int main(void){
    FILE * arquivo;
    arquivo = fopen("arquivo.bin","wb");
    gravacomp(2, &Teste1, "is05u",arquivo);
    fclose(arquivo);
    arquivo = fopen("arquivo.bin","rb");
    mostracomp(arquivo);
}
