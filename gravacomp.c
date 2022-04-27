#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função Principal

int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo);

// Funções para o cabeçalho

unsigned char IntHeader (unsigned char ContByte, unsigned char size, int isSigned);
unsigned char StringHeader (unsigned char ContByte, unsigned char size);

// Funções auxiliares

static unsigned char SizeSigned (int num);
static unsigned char SizeUnsigned (unsigned int num);
int string2num (char *s);


/****************************************************************************/

int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo){
  
  unsigned int ValueUnsigned; // Valores guardados no unsigned
  int ValueInt; // Valores guardados no signed
  unsigned char * AuxByte = (unsigned char *) valores; // Pegando os Bytes dos valores da struct
  char char_tamanho[3]; // Usado para calcular o número da string do struct
  unsigned char HeaderMontado; // Header montado para ser inserido no arquivo
  int tamanho_s = 0; // Tamanho da string do struct
  unsigned char ContByte; // Indica se eh o ultimo da estrutura
  unsigned int sizeByte; // Caso seja uma string tem o tamanho da string e caso seja um int tem o valor do numero de bytes usado para representar o int

  fwrite(&nstructs,sizeof(unsigned char),1,arquivo);  // PRIMEIRO BYTE TEM QUE SER A QUANTIDADE DE STRUCTS OU SEJA
  
  while(nstructs){
    for (int i = 0; i < strlen(descritor); i++){
      if (strlen(descritor) == i){
        ContByte = 1;
      }
    // string acompanha s00, int i, unsigned u, ou seja string 3 char int 1 char e unsigned 1 char
      
      switch (descritor[i]){

          case 's':
              //definir tamanho da string
              char_tamanho[0] = descritor[i+1];
              char_tamanho[1] = descritor[i+2];
              char_tamanho[2] = '\0';
              tamanho_s = string2num(char_tamanho);
              i+=2;
              HeaderMontado = StringHeader(ContByte, sizeByte);
              fwrite(&HeaderMontado,sizeof(unsigned char),1,arquivo); // Colocando o cabeçalho no arquivo
              break;

          case 'i':
            ValueInt = *((int*)AuxByte);
            sizeByte =  SizeSigned(ValueInt);
            HeaderMontado = IntHeader(ContByte,sizeByte,1);
            fwrite(&HeaderMontado,sizeof(unsigned char),1,arquivo); // Colocando o cabeçalho no arquivo
            break;

          case 'u':
            ValueUnsigned = *((unsigned int*)AuxByte);
            sizeByte = SizeUnsigned(ValueUnsigned);
            HeaderMontado = IntHeader(ContByte,sizeByte,0);
            fwrite(&HeaderMontado,sizeof(unsigned char),1,arquivo); // Colocando o cabeçalho no arquivo
            break;   
      }
    }
    ContByte = 0;
    nstructs--;
  }
  return 0;
}


int string2num (char *s) {
  int a = 0;
  for (; *s; s++)
    a = a*10 + (*s - '0');
  return a;
}

unsigned char IntHeader (unsigned char ContByte, unsigned char size, int IsSigned){
  unsigned char aux = 0; // 00000000
  aux = aux | size;
  if (ContByte == 1){
    aux = aux | (1<<7);
  }
  if (IsSigned){
    aux = aux | (1<<5);
    aux = aux & (0<<6);
  }
  else{
    aux = aux & (0<<6);
    aux = aux & (0<<5);
  }
  return aux;
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


// FUNÇÃO COPIADA DA INTERNET PARA TESTES, ENTENDER O QUE ELA FAZ
static unsigned char SizeUnsigned (unsigned int num){
    char i = 31;
    while (i--){
        if ((num & (1<<i)) == (1<<i))
            break;
    }
    if (i<8)
        return 1;
    else if (i<16)
        return 2;
    else if (i<24)
        return 3;
    return 4;
}
static unsigned char SizeSigned (int num){
    char i = 31;
    if ((num & (1<<i)) == (1<<i)){
        while (i--){
            if ((num & (1<<i)) != (1<<i))
                break;
        }
        if (i<7)
            return 1;
        else if (i<15)
            return 2;
        else if (i<23)
            return 3;
        return 4;
    }
    else return SizeUnsigned (num);
}