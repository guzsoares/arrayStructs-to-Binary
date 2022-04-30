#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função Principal

int gravacomp(int nstructs, void *valores, char *descritor, FILE *arquivo);

void mostracomp(FILE *arquivo);

// Funções para o cabeçalho gravacomp

unsigned char intHeader(unsigned char ContByte, unsigned char size, int isSigned); // Monta o header do int
unsigned char stringHeader(unsigned char ContByte, unsigned char size); // Monta o header da string

// Funções auxiliares para gravacomp

static unsigned char sizeSigned(int num); // Calcula a quantidade de bytes real ocupada por um signed
static unsigned char sizeUnsigned(unsigned int num); // Calcula a quantidade de bytes real ocupada por um unsigned
unsigned char FixPadding(int pad); // Funcao para calcular quantas casas terao que ser puladas para ir para o proximo elem da struct sem cair no padding
int string2num(char *s); // String to number padrao

// Funcoes auxiliares para mostracomp

int structEndCheck( unsigned char header);
char typeCheck(unsigned char header); 
unsigned char getNumBytes(unsigned char header, char type);
int isSigned(unsigned char MostValubleByte);

// Funcoes para debug
void dump (void *p, int n);

/***********************************************************************************************************************************************************************************************/

int gravacomp(int nstructs, void *valores, char *descritor, FILE *arquivo){
  
  int ContaPadding = 0; /* Usado para contar o padding entre os elementos da struct*/
  unsigned char aux1; /* unsigned char auxiliar para armazenar os valores no arquivo*/
  unsigned int ValueUnsigned; // Valor do unsigned
  int ValueInt; // Valor do signed
  unsigned char *AuxByte = (unsigned char *)valores; // Transformando a void valores em unsigned char para poder usar aritmetica de ponteiros
  char char_tamanho[3]; // Usado para calcular o tamanho da string na struct que eh representada da seguinte forma s01
  unsigned char HeaderMontado; // Header montado para ser inserido no arquivo
  int tamanho_s = 0; // Tamanho da string do struct
  unsigned char ContByte; // Indica se eh o ultimo da estrutura
  unsigned char sizeByte; // tamanho de bytes real que sao ocupados

  fwrite(&nstructs, sizeof(unsigned char), 1, arquivo);  // Primeiro byte indicando o nstructs
  
  while (nstructs) {
    for (int i = 0; i < strlen(descritor); i++) {
      if (strlen(descritor) == (i+1)) {
        ContByte = 1;
      }
      
      switch (descritor[i]) {

          case 's': // Caso seja str
              //definir tamanho da string
              char_tamanho[0] = descritor[i+1];
              char_tamanho[1] = descritor[i+2];
              char_tamanho[2] = '\0';
              tamanho_s = string2num(char_tamanho);
              
              // Pegando tamanho que a string ocupa e montando o header
              sizeByte = strlen((const char*)AuxByte);
              HeaderMontado = stringHeader(ContByte, sizeByte);
              fwrite(&HeaderMontado, sizeof(unsigned char), 1, arquivo); // Colocando o header no arqiuvo
              
              // adicionando os char no arquivo e calculando o padding para pular para o proximo valor
              fwrite(AuxByte, sizeof(unsigned char), sizeByte, arquivo);
              
              if (descritor[i-1] != 's') {
                  ContaPadding = FixPadding(tamanho_s);
                  AuxByte += ContaPadding;
              } else {
                  AuxByte += tamanho_s;
              }
              ContaPadding = 0;
              i += 2;
              break;

          case 'i': // Caso seja int
              
            ValueInt = *((int*)AuxByte); /* Associando o valor do int a uma variavel */
            sizeByte =  sizeSigned(ValueInt); /* Tamanho real que o int ocupa */
            HeaderMontado = intHeader(ContByte, sizeByte, 1); /* Montagem do header */
            fwrite(&HeaderMontado, sizeof(unsigned char), 1, arquivo); /* Colocando o header no arquivo */
              
            while (sizeByte) {
              aux1 = ((ValueInt)>>(8*(sizeByte-1))); /* Escrevendo os valores em Big Endian */
              fwrite(&aux1, sizeof(unsigned char), 1, arquivo);
              sizeByte--;
            }
            AuxByte += 4; /* Percorrendo os valores do struct*/
            break;

          case 'u': /* Caso seja unsigned int */
            ValueUnsigned = *((unsigned int*)AuxByte); /* Associando o valor do unsigned a uma variavel */
            sizeByte = sizeUnsigned(ValueUnsigned); /* Tamanho real que o unsigned ocupa */
            HeaderMontado = intHeader(ContByte, sizeByte, 0); /* Montando o header */
            fwrite(&HeaderMontado, sizeof(unsigned char), 1, arquivo); /* Colocando o header no arquivo */
              
            while (sizeByte) {
              aux1 = ((ValueUnsigned)>>(8*(sizeByte-1))); /* Escrevendo em big endian no arquivo */
              fwrite(&aux1, sizeof(unsigned char), 1, arquivo);
              sizeByte--;
            }
            AuxByte += 4; /* Percorrendo o ponteiro com os valores do struct */
            break;   
      }
    }
    ContByte = 0; /* Preparando o ContByte para a proxima struct */
    nstructs--; /* Avancando na struct */
  }
  return 0;
}


int string2num(char *s) { /* Funcao padrao str to num */
  int a = 0;
  for (; *s; s++)
    a = a*10 + (*s - '0');
  return a;
}

unsigned char intHeader(unsigned char ContByte, unsigned char size, int IsSigned) { /* Montagem do Header do int*/
  unsigned char aux = 0; /* Criando um byte 0000 0000 */
  aux = aux | size; /* Colando o byte do size no aux */
  if (ContByte == 1) { /* Caso seja o ultimo iremos inserir o 1 no setimo bit */
    aux = aux | (1<<7);
  }
  if (IsSigned) { /* Caso seja signed iremos inserir o 1 no quinto bit */
    aux = aux | (1<<5);
  }
  return aux; /* Header montado */
}

unsigned char stringHeader (unsigned char ContByte, unsigned char size) { /* Montagem do Header do str */
  unsigned char aux = 0; /* Criando um byte 0000 0000 */
  aux = aux | size; /* Colando o byte do size no aux */
  if (ContByte == 1) { /* Caso seja o ultimo iremos inserir o 1 no setimo bit */
    aux = aux | (1<<7);
  }
  aux = aux | (1<<6); /* Inserindo o 1 no 6 bit pois eh necessario */
  return aux;
}


static unsigned char sizeUnsigned(unsigned int num) { /* REFAZER FUNCAO PARA TERMOS A NOSSA VERSAO */
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
static unsigned char sizeSigned (int num){ /* REFAZER FUNCAO PARA TERMOS A NOSSA VERSAO */
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
    else return sizeUnsigned (num);
}

unsigned char FixPadding(int pad) {
    while (pad%4 != 0) { // ENQUANTO O NUMERO NAO FOR DIVISIVEL POR 4 QUE SAO OS BYTES EM UM INT ELE NAO SERA COMPATIVEL, POIS ELE IRA CAIR EM UM PADDING
        pad++;
    }
    return pad;
}

void mostracomp(FILE * arquivo) {
    char str[64];
    int nstructs;
    unsigned char header;
    char type;
    int contByte = 0;
    unsigned char numBytes;
    int i = 0;

    /*Variaveis para guardar os bytes*/
    unsigned char chari;
    unsigned char IntBytes[4]; 
    int num = 0;  //int que irá ser printado
    int isPositive;

    //Debug
    int qualbyteEstou = 0;
    int gusacoint;
    unsigned int gusacoU;
    nstructs = fgetc(arquivo); // Primeiro byte representa o numero de estruturas
    qualbyteEstou++;
    
    printf("Estruturas: %d \n", nstructs);
    
    while (nstructs) {
        while(!contByte){
          header = fgetc(arquivo); // Segundo byte eh o cabecalho
          qualbyteEstou ++;
          contByte = structEndCheck(header);
          type = typeCheck(header);
          numBytes = getNumBytes(header,type);
          unsigned char IntByte[4] = { 0 };
          switch (type)
          {
            case 's':
              for (i = 0; i < numBytes ; i++) {
                chari = fgetc(arquivo);
                qualbyteEstou++;
                str[i] = chari;
              }

              str[i] = '\0';
              printf("(str) %s\n",str);
              
              break;

            case 'i':
              for ( i = 0; i < numBytes; i++) {
                IntBytes[i] = fgetc(arquivo);  //Pega o proximo char do arquivo
                qualbyteEstou++;
                if (i == 0){
                  isPositive = isSigned(IntBytes[0]); //observa o últmo bit para ver se o número é negativo
                }
                num = (num << 8) | IntBytes[i]; //Monta o int através do bitshift
              }
              if (!isPositive){
                num = num | (-1<<(8 * numBytes)); //Se o número 
              }
              printf("(int) %d (%x)\n",num,num);
              break;

            case 'u':
              for ( i = 0; i < numBytes; i++) {

              }
              printf("estou no u\n");
              gusacoU = fgetc(arquivo);
              qualbyteEstou++;
              break;

          }
        }
    nstructs--;
    }
}

char typeCheck(unsigned char header) {
  if ((header & (1<<6)) == (1<<6)) {
    return's';
  }
  else if ((header & (1<<5)) == (1<<5)) {
    return 'i';
  } else {
    return 'u';
  }
}

int structEndCheck(unsigned char header) {
  if ((header & 128) == 128) {
    return 1;
  } else {
    return 0;
  }
}
unsigned char getNumBytes(unsigned char header, char type) {
  unsigned char aux;
  if (type == 's') {
    aux = 63; //aux = 0011 1111
  }
  else {
    aux = 31; //aux = 0001 1111
  }  
  aux = aux & header;   //retorna os bits necessarios
  return aux;
}
int isSigned(unsigned char MostValubleByte){ //Para verificar se o número é positivo ou negativo
  if((MostValubleByte & (1<<8)) == (1<<8)){
    return 1;
  } else {
    return 0;
  }
}

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

  
