#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gravacomp.h"

struct teste1 {
    int  i1;
    char s1[5];
    char s2[3];
    unsigned int u1;
}Teste1[2] = {{-1,"abc", "f",258},{1,"ABCD", "kk",65535}};

int main(void){
    FILE * arquivo;
    arquivo = fopen("arquivo.bin","wb");
    gravacomp(2, &Teste1, "is05s03u",arquivo);
    fclose(arquivo);
    arquivo = fopen("arquivo.bin","rb");
    mostracomp(arquivo);
}
