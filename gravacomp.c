/*Lucca Vieira Rocha 2011342 3WA*/
/*Gustavo Molina Soares 2020209 3WA*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função Principal

int gravacomp(int nstructs, void *valores, char *descritor, FILE *arquivo);

void mostracomp(FILE *arquivo);

// Funções para o cabeçalho gravacomp

unsigned char intHeader(unsigned char contByte, unsigned char size, int isSigned); // Monta o header do int
unsigned char stringHeader(unsigned char contByte, unsigned char size); // Monta o header da string

// Funções auxiliares para gravacomp

static unsigned char sizeSigned(int num); // Calcula a quantidade de bytes real ocupada por um signed
static unsigned char sizeUnsigned(unsigned int num); // Calcula a quantidade de bytes real ocupada por um unsigned
unsigned char fixPadding(int pad); // Funcao para calcular quantas casas terao que ser puladas para ir para o proximo elem da struct sem cair no padding
int string2num(char *s); // String to number padrao
int alinhamentoAuxByte(unsigned char *origin, unsigned char *ptr);

// Funcoes auxiliares para mostracomp

int structEndCheck( unsigned char header);
char typeCheck(unsigned char header); 
unsigned char getNumBytes(unsigned char header, char type);
int isSigned(unsigned char MostValubleByte);


/***********************************************************************************************************************************************************************************************/

int gravacomp(int nstructs, void *valores, char *descritor, FILE *arquivo){
  
  unsigned char aux1; /* unsigned char auxiliar para armazenar os valores no arquivo*/
  unsigned int valueUnsigned; // Valor do unsigned
  int valueInt; // Valor do signed
  unsigned char *auxByte = (unsigned char *)valores; // Transformando a void valores em unsigned char para poder usar aritmetica de ponteiros
  char charSize[3]; // Usado para calcular o tamanho da string na struct que eh representada da seguinte forma s01
  unsigned char headerMontado; // Header montado para ser inserido no arquivo
  int tamanhoS = 0; // Tamanho da string do struct
  unsigned char contByte; // Indica se eh o ultimo da estrutura
  unsigned char sizeByte; // tamanho de bytes real que sao ocupados
  unsigned char *originAddress = auxByte; //Copia o endereço inicial de auxByte
  int positionStruct;

  if(fwrite(&nstructs, sizeof(unsigned char), 1, arquivo) != 1){ // Primeiro byte indicando o nstructs
    return -1; //Se o fwrite falhar o retorno da função é -1
  }  
  while (nstructs) {
    for (int i = 0; i < strlen(descritor); i++) {
      if (strlen(descritor) == (i + 1)) { //Detecta se é o último elemento no caso 'i' e 'u'
        contByte = 1;
      }
      else if ((descritor[i] == 's') && (strlen(descritor) - i) == 3) { //Detecta se é o último elemento no caso 's'
        contByte = 1;
      }
      
      
      switch (descritor[i]) {

          case 's': // Caso seja str
              //definir tamanho da string
              charSize[0] = descritor[i + 1];
              charSize[1] = descritor[i + 2];
              charSize[2] = '\0';
              tamanhoS = string2num(charSize);
              
              // Pegando tamanho que a string ocupa e montando o header
              sizeByte = strlen((char*)auxByte);
              headerMontado = stringHeader(contByte, sizeByte);

              // Colocando o header no arqiuvo
              if(fwrite(&headerMontado, sizeof(unsigned char), 1, arquivo) != 1){
                return -1;
              } 
              
              // adicionando os char no arquivo e calculando o padding para pular para o proximo valor
              if(fwrite(auxByte, sizeof(unsigned char), sizeByte, arquivo) != sizeByte){
                return -1;
              }
              
              //Avança os bytes do struct no tamanho do tamanhoS
              auxByte += tamanhoS;
              i += 2;
              break;

          case 'i': // Caso seja int
            //Para realizar o alinhamento eu calculei a posição do byte atual no programa 
            positionStruct = alinhamentoAuxByte(originAddress, auxByte);
            //E passei pelo fixPadding para adicionar o padding necessário e alinhar o auxByte
            auxByte = auxByte + fixPadding(positionStruct);


            valueInt = *((int*)auxByte); /* Associando o valor do int a uma variavel */
            sizeByte =  sizeSigned(valueInt); /* Tamanho real que o int ocupa */
            headerMontado = intHeader(contByte, sizeByte, 1); /* Montagem do header */
            if(fwrite(&headerMontado, sizeof(unsigned char), 1, arquivo) != 1) { /* Colocando o header no arquivo */
              return -1;
            }
              
            while (sizeByte) {
              aux1 = ((valueInt) >> (8*(sizeByte - 1))); /* Escrevendo os valores em Big Endian */
              if(fwrite(&aux1, sizeof(unsigned char), 1, arquivo) != 1){
                return -1;
              }
              sizeByte--;
            }
            auxByte += 4; /* Percorrendo os valores do struct*/
            break;

          case 'u': /* Caso seja unsigned int */
            //Para realizar o alinhamento eu calculei a posição do byte atual no programa 
            positionStruct = alinhamentoAuxByte(originAddress, auxByte);
            //E passei pelo fixPadding para adicionar o padding necessário e alinhar o auxByte
            auxByte = auxByte + fixPadding(positionStruct);

            valueUnsigned = *((unsigned int*)auxByte); /* Associando o valor do unsigned a uma variavel */
            sizeByte = sizeUnsigned(valueUnsigned); /* Tamanho real que o unsigned ocupa */
            headerMontado = intHeader(contByte, sizeByte, 0); /* Montando o header */
            if(fwrite(&headerMontado, sizeof(unsigned char), 1, arquivo) != 1){ /* Colocando o header no arquivo */
              return -1;
            } 
              
            while (sizeByte) {
              aux1 = ((valueUnsigned)>>(8*(sizeByte-1))); /* Escrevendo em big endian no arquivo */
              if(fwrite(&aux1, sizeof(unsigned char), 1, arquivo) != 1) {
                return -1;
              }
              sizeByte--;
            }
            auxByte += 4; /* Percorrendo o ponteiro com os valores do struct */
            break;   
      }
    }
    contByte = 0; /* Preparando o contByte para a proxima struct */
    nstructs--; /* Avancando na struct */
  }
  return 0;
}

int alinhamentoAuxByte(unsigned char *origin, unsigned char *ptr){  //Calula a posição do ptr em relação a sua origem
  if (origin == ptr){
    return 0;
  }
  int diff = ptr - origin;
  return diff;
}


int string2num(char *s) { /* Funcao padrao str to num */
  int a = 0;
  for (; *s; s++)
    a = a*10 + (*s - '0');
  return a;
}

unsigned char intHeader(unsigned char contByte, unsigned char size, int IsSigned) { /* Montagem do Header do int*/
  unsigned char aux = 0; /* Criando um byte 0000 0000 */
  aux = aux | size; /* Colando o byte do size no aux */
  if (contByte == 1) { /* Caso seja o ultimo iremos inserir o 1 no setimo bit */
    aux = aux | (1<<7);
  }
  if (IsSigned) { /* Caso seja signed iremos inserir o 1 no quinto bit */
    aux = aux | (1<<5);
  }
  return aux; /* Header montado */
}

unsigned char stringHeader (unsigned char contByte, unsigned char size) { /* Montagem do Header do str */
  unsigned char aux = 0; /* Criando um byte 0000 0000 */
  aux = aux | size; /* Colando o byte do size no aux */
  if (contByte == 1) { /* Caso seja o ultimo iremos inserir o 1 no setimo bit */
    aux = aux | (1<<7);
  }
  aux = aux | (1<<6); /* Inserindo o 1 no 6 bit pois eh necessario */
  return aux;
}


unsigned char sizeUnsigned(unsigned int num){
  int bytes = 0;
  int i = 0;
  int j = 0;
  if(num == 0) {
    return 1;
  }
  for(j = 0; j<4;j++){
    if((num >> i) != 0){
      bytes++;
    }
    i+=8;
  }
  return bytes;
}
unsigned char sizeSigned (int num){ //Retorna o tamanho real de um int
    char i = 31;
	int bytes = 0;
    if(num == 0){
      return 1;
    }
    if(num>=128 && num<=255){
		  return 2;
	  }
    if(num>=32768 && num<=65535){
      return 3;
    }
    if(num >= 8388608 && num<= ((8388608*2)-1)){
      return 4;
    }
    if (num < 0) {
        while (i--){
            if ((num & (1<<i)) != (1<<i))
                break;
        }
        if (i<7) {
			bytes = 1;
            return bytes;
		}
        else if (i<15) {
			bytes = 2;
            return bytes;
		}
        else if (i<23) {
			bytes = 3;
            return bytes;
		}
		bytes = 4;
        return bytes;
    }
	else {
		return sizeUnsigned(num);
	}
}


unsigned char fixPadding(int pad) {
    int soma = 0;
    if (pad == 0) {
        return soma;
    }
    while (pad%4 != 0) { // ENQUANTO O NUMERO NAO FOR DIVISIVEL POR 4 QUE SAO OS BYTES EM UM INT ELE NAO SERA COMPATIVEL, POIS ELE IRA CAIR EM UM PADDING
        pad++;
        soma++;
    }
    return soma;
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
    int num = 0;  //int que irá ser printado
    int isNegative;

    nstructs = fgetc(arquivo); // Primeiro byte representa o numero de estruturas
  
    
    printf("Estruturas: %d \n", nstructs);
    
    while (nstructs) {
      contByte = 0; //Como existe outro struct na fila o contbyte é zerado até o últmo struct onde ele fica como 1
      printf("\n");
        while(!contByte){
          header = fgetc(arquivo); // Segundo byte eh o cabecalho
          contByte = structEndCheck(header);
          type = typeCheck(header);
          numBytes = getNumBytes(header,type);
          unsigned char IntBytes[4] = { 0 };
          num = 0;

          switch (type)
          {
            case 's':
              for (i = 0; i < numBytes ; i++) {
                chari = fgetc(arquivo);
                //monta a string de acordo com cada byte no arquivo
                str[i] = chari;
              }

              str[i] = '\0';
              printf("(str) %s\n",str);
              
              break;

            case 'i':
              for ( i = 0; i < numBytes; i++) {
                IntBytes[i] = fgetc(arquivo);  //Pega o proximo char do arquivo
              
                if (i == 0){
                  isNegative = isSigned(IntBytes[0]); //observa o últmo bit para ver se o número é negativo
                }
                num = (num << 8) | IntBytes[i]; //Monta o int através do bitshift
              }
              if (isNegative){
                num = num | (-1<<(8 * numBytes)); //Se o número for negativo as casas na esquerda viram 1
              }
              printf("(int) %d (%08x)\n",num,num);
              break;

            case 'u':
              for ( i = 0; i < numBytes; i++) {
                IntBytes[i] = fgetc(arquivo);
                num = (num << 8) | IntBytes[i];
              }
              printf("(uns) %u (%08x)\n",num,num);
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
  if ((header & 128) == 128) {              //128 = 1000 0000
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
  if((MostValubleByte & 128) == 128) {        //128 = 1000 0000
    return 1;
  } else {
    return 0;
  }
}
