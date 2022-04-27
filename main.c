#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gravacomp.h"

int main(void){
    FILE * arquivo;
    arquivo = fopen("arquivo","wb");
    gravacomp(...)
    fclose(arquivo);
}