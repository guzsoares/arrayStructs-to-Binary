# T1-software-basico
Trabalho 1 de software básico PUC-Rio

## Armazenamento compactado

O objetivo do trabalho é implementar, na linguagem C, uma função (**`gravacomp`**) que armazena um array de structs em um arquivo binário de forma compactada e uma função (**`mostracomp`**) que permite visualizar um arquivo gerado por **`gravacomp`** na saída padrão (terminal).

## Função `gravacomp`

```c
int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo);
```

A função **`gravacomp`** recebe:

- `nstructs`: o número de elementos do array de structs
a ser armazenado em arquivo
- `valores`: um ponteiro para o array de structs propriamente dito
- `descritor`: uma descrição dos campos das structs que compõem o array
- `arquivo`: um arquivo aberto para escrita, em modo binário

A string **`descritor`** representa o tipo de cada um dos campos das structs (na ordem correeta), conforme abaixo:

```c
    's' - string (char [])
    'u' - inteiro sem sinal (unsigned int)
    'i' - inteiro com sinal (signed int)
```

Para campos do tipo *string*, deve-se indicar o tamanho do array, **sempre com dois dígitos**. O tamanho máximo das *strings* armazenadas nas estruturas, por restrição imposta pelo formato do arquivo (como veremos mais adiante), é 63, descontado o marcador de fim (`\0`). Portanto, o tamanho máximo do array é limitado a 64.

Por exemplo, dada a declaração:

```
  struct s {
    int  i1, i2;
    char s1[3];
    unsigned int u1;
    char s2[10];
  };
struct s exemplo[5];

```

a string descritor correspondente é **`"iis03us10"`**

Assumindo que o arquivo de saída está armazenado em uma variável **`arq`**, do tipo **`FILE*`**, a chamada para a gravação compactada do array **`exemplo`** (após preenchido) seria:

```c
   res = gravacomp(5, exemplo, "iis03us10", arq);
```

### Formato do arquivo gerado

O formato do arquivo de saída é feito da seguinte maneira:

- O primeiro byte do arquivo indica o número de structs armazenadas (como
um *unsigned char*). Note que o número máximo de structs armazenadas no arquivo é, portanto, 255.
- A seguir deverão vir os campos de cada estrutura.
    
    Para cada campo, deverão ser gravados um *byte de cabeçalho* e, em seguida,
    os bytes que representam o conteúdo compactado do campo, conforme descrito
    a seguir.
    

**Armazenamento de strings**

Para campos do tipo *string*, o cabeçalho tem o seguinte formato:

| Bits: | 7 | 6 | 5-0 |
| --- | --- | --- | --- |
|  | cont | tipo | tamanho |

O bit mais significativo (**cont**) indica se este é o último campo da estrutura (1) ou não (0). O bit 6 (**tipo**) deverá conter o valor 1. Os bits 5-0 (**tamanho**) têm o tamanho (em número de bytes) da string armazenada a seguir.

Após o cabeçalho, devem ser gravados os bytes que compõem a string, **porém apenas os bytes anteriores ao marcador de fim de string (\0)**. Note que o tamanho da string armazenada pode variar de 1 a n-1, onde n é o tamanho do vetor que corresponde a esse campo na estrutura (pois o \0 nunca será gravado). Como temos apenas 6 bits para o tamanho da string, esse tamanho é limitado a 63.

Por exemplo, se temos um campo definido como **`char s2[10]`**, que não é o último campo da estrutura, e neste campo está armazenada a string **`"abc"`**, seu cabeçalho terá:

```
cont: 0
tipo: 1
tamanho: 3
```

e apenas três bytes serão gravados após o cabeçalho (os códigos dos caracteres 'a', 'b' e 'c').

**Armazenamento de inteiros com e sem sinal**

Para campos do tipo *inteiro*, o cabeçalho tem o seguinte formato:

| Bits: | 7 | 6-5 | 4-0 |
| --- | --- | --- | --- |
|  | cont | tipo | tamanho |

Novamente, o bit mais significativo (**cont**) indica se este é o último campo da estrutura (1) ou não (0). Os bits 4-0 (**tamanho**) têm o número de bytes usados para representar o valor e os bit 6-5 (**tipo**) deverão conter

00 - se o campo é do tipo **u** (unsigned int)
 01 - se o campo é do tipo **i** (signed int)

Após o cabeçalho, devem ser gravados, **em big endian**, os bytes que representam o valor do campo. Porém, ao invés de armazenar sempre quatro bytes, a função deverá gravar **apenas os bytes necessários para manter a representação do valor**.

Por exemplo, se temos um campo do tipo *signed int* com o valor **-1**, podemos usar apenas um byte para seu valor (um byte com valor **11111111** em binário, ou **FF** em hexadecimal). Desta forma, se esse campo é o último campo da estrutura, seu cabeçalho terá:

```
cont: 1
tipo: 01
tamanho: 1
```

e após o cabeçalho será gravado apenas um byte

Se o campo contiver o valor **258**, precisaremos de dois bytes para representar seu valor (o primeiro byte com valor **01** e o segundo com valor **02**, em hexadecimal).

Se o campo contiver o valor -65536 (-2^16), precisaremos de três bytes
(**FF**, **00** e **00**, nesta ordem).

---

Vejamos um exemplo de codificação completa. Suponha a seguinte estrutura:

```c
  struct s {
    int  i;
    char s1[5];
    unsigned int u;
  };
```

é um array com duas estruturas desse tipo. Se os campos da primeira estrutura contiverem, nesta ordem, os valores `-1`, `"abc`" e `258` e os da segunda os valores 1, `"ABCD"` e `65535`, o conteúdo armazenado para esse array será (com os valores dos bytes exibidos em hexadecimal):

`02 21 ff 43 61 62 63 82 01 02 21 01 44 41 42 43 44 82 ff ff`

---

## Função `mostracomp`

```c
void mostracomp (FILE *arquivo);
```

A função `mostracomp` permite a visualização de um arquivo criado por `gravacomp` na saída padrão (terminal). Essa saída pode ser gerada, por exemplo, através de chamadas a `printf`.

A saída da função `mostracomp` é feita da seguinte maneira:

- uma linha indicando o número de structs armazenadas no arquivo
- o tipo e o valor armazenado em cada campo
- uma quebra de linha deve ser inserida entre cada estrutura

Para campos do tipo *string* exibido **(str)** e a string propriamente dita (com formato %s).

Para campos do tipo *unsigned int* deve ser exibido **(uns)** e o valor original em decimal (formato %u) e, entre parênteses, em hexadecimal (formato %08x).

Para campos do tipo *signed int* deve ser exibido **(int)** e o valor original em decimal (formato %d) e, entre parênteses, em hexadecimal (formato %08x).

Para o arquivo do exemplo discutido acima, a saída de  `mostracomp`  seria

```
Estruturas: 2

(int) -1 (ffffffff)
(str) abc
(uns) 258 (00000102)

(int) 1 (00000001)
(str) ABCD
(uns) 65535 (0000ffff)
```
