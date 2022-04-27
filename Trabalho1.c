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
    char s2[10];
  };
struct s exemplo[5];


int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo);
int string2num (char *s);

// Funções para o cabeçalho

unsigned char IntHeader (unsigned char ContByte, int Value, unsigned char size, int isSigned);
unsigned char StringHeader (unsigned char ContByte, unsigned char size);

/****************************************************************************/
int main(void){
  void *p;
  char * descritor = "iis03us10";
  FILE *arquivo = fopen("arquivo.bin", "wb");
  if(!arquivo){
    fprintf(arquivo ,"Erro ao abrir arquivo\n");
    exit(1);
  }
  int a = gravacomp(2, p, descritor, arquivo);
  return 0;
}

int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo){
  int tamanho_s = 0;
  unsigned char PrimeiroByte;     // Quantidade de structs armazenadas
  unsigned char ContByte;          // Indica se eh o ultimo da estrutura
  unsigned int TypeByte;          // Caso seja um char devera conter 1 e caso seja um int devera conter 00 se eh unsigned e 01 se eh signed
  unsigned int sizeByte;          // Caso seja uma string tem o tamanho da string e caso seja um int tem o valor do numero de bytes usado para representar o int
  unsigned char StringByte;       // Usado para gravar os bytes que compoem a string
  unsigned char ValueByte;          // Usado para gravar os bytes usados para representar o int - lembrando de usar somente o necessario para armazenar o int e nao 4 bytes
  while(nstructs){
    for (int i = 0; i < strlen(descritor); i++){
    // string acompanha s00, int i, unsigned u, ou seja string 3 char int 1 char e unsigned 1 char
      
      switch (descritor[i]){
          case 's':
              printf("Achei uma string ");
              //definir tamanho da string
              char char_tamanho[3];
              char_tamanho[0] = descritor[i+1];
              char_tamanho[1] = descritor[i+2];
              char_tamanho[2] = '\0';
              //esse resultado que será impresso no arquivo
              
              
              printf("de tamanho %d \n",tamanho_s);
              
              i+=2;
              //se for um s, ele le a casa + 2 referentes ao numero de caracteres
              break;
          case 'i':
              printf("Achei um int \n");
              break;
          case 'u':
              printf("Achei um unsigned \n");
              break;   
      }
    }
    nstructs--;
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

unsigned char IntHeader (unsigned char ContByte, int Value, unsigned char size, int IsSigned){
  unsigned char aux = 0; // 00000000
  aux = aux | size;
  if (ContByte == 1){
    aux = aux | (1<<7);
  }
  if (IsSigned){
    aux = aux | (1<<5);
    aux = aux & (0<<6);
    return aux;
  }
  else{
    aux = aux & (0<<6);
    aux = aux & (0<<5);
    return aux;
  }
}

unsigned char StringHeader (unsigned char ContByte, unsigned char size){
  unsigned char aux = 0;
  aux = aux | size;
  if (ContByte == 1){
    aux = aux | (1<<7);
  }
  aux = aux | (1<<6);
  return aux;
}
