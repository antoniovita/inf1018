// Qual o tamanho (em bytes) de cada um dos arrays abaixo e como você acha que eles são armazenados em memória?

#include <stdio.h>

int b[2]; // 4 bytes para cada int, ou seja 8 bytes
short a[2][3]; // 2 bytes para cada short, como é uma matrix de 2x3 = 6 shorts, então 12 bytes

// 3) R: Eles são armazenados consecutivamente em memória.

struct X {
  int a; //4 bytes
  short b; //2 bytes
  int c; // 4 bytes
} x = {0xa1a2a3a4, 0xb1b2, 0xc1c2c3c4};

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  int i,j;

  for (i=0;i<2;i++) {
    b[i] = i;
    for (j=0;j<3;j++)
      a[i][j] = 3*i+j;
  }

  printf ("x: \n");
  dump (&x, sizeof(x));
  printf("sizeof(x): %d\n", sizeof(x));

  return 0;
}

// 2) R: A função dump imprimiu bytes nulos após os dois bytes do short, isso ocorre pois a linguagem C passa todos para int, o que faz o short que tinha 2 bytes virar um int de 4 bytes, esses bytes excedentes sao preenchidos com bytes nulos 00


// Qual o tamanho, e como os tipos abaixo são organizados na memória?
// Depois de escrever suas respostas use a função dump para conferí-las. Uma sugestão de programa de teste, com valores "reconhecíveis" para os campos das estruturas está aqui.

 
struct X1 {
  char c1;
  int i;
  char c2;
};

// R: O tamanho desse struct é 12 bytes, pois pega-se o tamanho do maior type, no caso int que sao 4 bytes, e multiplica-se pela qunatidade elementos
 
struct X2 {
  long l;
  char c;
};
 
// R: O tamanho desse struct é 8 bytes, pois pega-se o tamanho do maior type, no caso long, que sao 8bytes e multiplica-se pela qunatidade elementos

struct X3 {
  int i;
  char c1;
  char c2;
};
 
// R: O tamanho desse struct é 12 bytes, pois pega-se o tamanho do maior type, no caso int, que sao 4bytes e multiplica-se pela qunatidade elementos

struct X4 {
  struct X2 x;
  char c;
};

// R: O tamanho desse struct é 24 bytes, pois pega-se o tamanho do maior type, no caso a struct x2, que sao 12 bytes e multiplica-se pela qunatidade elementos

 
struct X5 {
  char c1;
  char c2;
  char c3;
};

// R: O tamanho desse struct é 3 bytes, pois pega-se o tamanho do maior type, que é o char que tem 1 byte e emultiplica-se pela quantidade de elementos
 
struct X6 {
  short s1;
  int i;
  char c[3];
  short s2;
};

// R: O tamanho desse struct é 16 bytes, pois pega-se o tamanho do maior type, que é o int que tem 4 bytes e emultiplica-se pela quantidade de elementos
 
union U1 {
  int i;
  char c[5];
};

//R: Para uma union pega-se o maior multiplo que faca uma divisao inteira para os bytes, no caso 8, visto que 4 bytes nao da para o char[5] que precisa de 5 bytes, entao 2x4 = 8bytes


 
union U2 {
  short s;
  char c[5];
};

//R: Para uma union pega-se o maior multiplo que faca uma divisao inteira para os bytes, no caso 6, visto que 2 bytes nao da para o char[5] que precisa de 5 bytes, entao 3x2 = 6 bytes
