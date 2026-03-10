#include <stdio.h>
// int main(void) {
//   unsigned int x = 0x87654321;
//   unsigned int y, z;

//   y = x & 0x000000FF;
//   z = x | 0xFF000000;

//   printf("%08x %08x\n", y, z);
// }


// int odd_ones(unsigned int x) {
//   int count = 0;

//   while (x) {
//     count += x & 1;
//     x >>= 1;
//   }

//   return count & 1;
// }

// int main() {
//   printf("%x tem numero %s de bits\n",0x01010101, odd_ones(0x01010101) ? "impar":"par");
//   printf("%x tem numero %s de bits\n",0x01030101, odd_ones(0x01030101) ? "impar":"par");
//   return 0;
// }


// unsigned char switch_byte(unsigned char x) {
//   return (x >> 4) | (x << 4);
// }

// int main() {
//   printf("%x", switch_byte(0xAB));
//   return 0;
// }

unsigned char rotate_left(unsigned char x, int n) {
    return (x << n) | (x >> (8 - n));
}

int main() {
    printf("%x\n", rotate_left(0x61, 2));
    return 0;
}