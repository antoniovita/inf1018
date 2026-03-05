#include <ctype.h>
#include <stdio.h>

/*
void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  short i = 4;
  dump(&i, sizeof(i));
  return 0;
}
*/

/*
int string2num (char *s) {
  int a = 0;
  for (; *s; s++)
    a = a*10 + (*s - '0');
  return a;
}
*/

#include <ctype.h>

int string2num(const char *s, int base) {
  int a = 0;

  if (base < 2 || base > 36) return 0;

  for (; *s && *s != '\n'; s++) {
    unsigned char uc = (unsigned char)*s;
    int dig;

    if (isdigit(uc)) {
      dig = uc - '0';                 /* '0'..'9' -> 0..9 */
    } else if (isalpha(uc)) {
      dig = tolower(uc) - 'a' + 10;   /* 'a'..'z' -> 10..35 */
    } else {
      return 0;                       /* caractere invalido */
    }

    if (dig >= base) return 0;        /* digito nao existe nessa base */
    a = a * base + dig;
  }

  return a;
}


int main (void) {


  26
41115
1633295
  return 0;
}