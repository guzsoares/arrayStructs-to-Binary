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

struct teste2 {
    int  i1;
    char s1[8];
	char s2[3];
    unsigned int u1;
}Teste2[2] = {{-35,"abc","ax",258},{1,"ABCD","av",65535}};

struct teste3 {
	char s3[5];
    int  i1;
    char s1[12];
	char s2[6];
    unsigned int u1;
}Teste3[3] = {{"123x",35,"abcddddqw","ax",258},{"1224",1,"ABzxcCD","av",65535},{"foda",423,"ABzxcCD","axsv",7003}};

struct teste4 {
    int  i1;
    int  i2;
	int  i3;
	int  i4;
}Teste4[2] = {{-354,234,111,-200},{1,34,-127,24}};

struct teste5 {
    int  i1;
    char s1[5];
	char s2[3];
}Teste5[2] = {{-23,"acb","ab"},{42,"aaa","bb"}};


int main(void){
    FILE * arquivo;
    arquivo = fopen("arquivo.bin","wb");
    //gravacomp(2, &Teste1, "is05s03u",arquivo);
	/* TESTES */
  /*gravacomp(2, &Teste2, "is05u",arquivo);  // PASS
    gravacomp(2, &Teste2, "is08s03u",arquivo); // PASS
	gravacomp(3, &Teste3, "s05is12s06u",arquivo); // PASS
	gravacomp(2, &Teste4, "iiii",arquivo); // PASS
	gravacomp(2, &Teste5, "is05s03",arquivo); */ // PASS
    gravacomp(2, &Teste2, "is08s03u",arquivo);
    fclose(arquivo);
    arquivo = fopen("arquivo.bin","rb");
    mostracomp(arquivo);
}
