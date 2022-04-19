#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TABELA ASCII:
a = 0x61; b = 0x62; c = 0x63;
*/

struct s {
    int  i1, i2;
    char s1[3];
    unsigned int u1;
  };
struct s exemplo[1];


int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo);
int string2num (char *s);

/****************************************************************************/
int main(void){
  void *p;
  char * descritor = "iis03u";
  FILE *arquivo = fopen("arquivo.bin", "wb");
  if(!arquivo){
    fprintf(arquivo ,"Erro ao abrir arquivo\n");
    exit(1);
  }
  int a = gravacomp( 1, p, descritor, arquivo);
  return 0;
}

int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo){
  // da pra fazer ao inves de um "for", um "while(descritor[i] != '\0')"
    int tamanho_s = 0;
  for (int i = 0; i < strlen(descritor); i++){
    // string acompanha s00, int i, unsigned u, ou seja string 3 char int 1 char e unsigned 1 char
      switch (descritor[i]){
          case 115: // ASCII table s = 115, i = 105, u = 117
              printf("Achei uma string ");
              //definir tamanho da string
              char char_tamanho[3];
              char_tamanho[0] = descritor[i+1];
              char_tamanho[1] = descritor[i+2];
              char_tamanho[2] = '\0';
              int size = string2num(char_tamanho);
              
              tamanho_s = size %4;
              //esse resultado que será impresso no arquivo
              
              
              printf("de tamanho %d \n",tamanho_s);
              
              i+=2;
              //se for um s, ele le a casa + 2 referentes ao numero de caracteres
              break;
          case 105:
              printf("Achei um int \n");
              break;
          case 117:
              printf("Achei um unsigned \n");
              break;
          
      }
  }
  
  
  
  if (arquivo == NULL){
    printf("Erro ao gravar no arquivo\n");
   return -1;
  }
  
  return 0;
}


int string2num (char *s) {
  int a = 0;
  for (; *s; s++)
    a = a*10 + (*s - '0');
  return a;
}
