/* Antonio Vita Fonseca 2510071 3WC */
/* David Faerman 2510179 3WC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <limits.h>
#include "cria_func.h"

#define BUF 256

typedef int (*func0)();
typedef int (*func1)(int);
typedef int (*func2)(int,int);
typedef int (*func3)(int,int,int);

static int soma(int a, int b)         { return a + b; }
static int soma3(int a, int b, int c) { return a + b + c; }
static int identidade(int x)          { return x; }
static int escreve_ptr(void *p)       { return *(int*)p; }
static int mul(int a, int b)          { return a * b; }
static int sub(int a, int b)          { return a - b; }
static int retorna_zero()             { return 0; }

#ifndef MAP_JIT
#define MAP_JIT 0
#endif

static void *aloca_exec(unsigned char *buf, int n) {
    void *mem = mmap(NULL, n, PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_ANON|MAP_PRIVATE|MAP_JIT, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); exit(1); }
    memcpy(mem, buf, n);
    return mem;
}

int main(void) {
    unsigned char codigo[BUF];
    int ok = 0, falhou = 0;

#define CHECK(desc, got, esperado) \
    if ((got) == (esperado)) { printf("  [OK] %s => %d\n", desc, (int)(got)); ok++; } \
    else { printf("  [FALHOU] %s => got %d, esperado %d\n", desc, (int)(got), (int)(esperado)); falhou++; }

    /* 1 */
    printf("=== Teste 1: FIX(INT)+FIX(INT) -> soma(3,7) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=3}},{INT_PAR,FIX,{.v_int=7}}};
        cria_func(soma, p, 2, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("soma(3,7)", g(), 10);
        munmap(g, BUF);
    }

    /* 2 */
    printf("=== Teste 2: PARAM+FIX(INT) -> soma(x,100) ===\n");
    {
        DescParam p[] = {{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,FIX,{.v_int=100}}};
        cria_func(soma, p, 2, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("soma(5,100)",  g(5),  105);
        CHECK("soma(-3,100)", g(-3),  97);
        munmap(g, BUF);
    }

    /* 3 */
    printf("=== Teste 3: FIX(INT)+PARAM -> soma(42,x) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=42}},{INT_PAR,PARAM,{.v_int=0}}};
        cria_func(soma, p, 2, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("soma(42,8)", g(8), 50);
        CHECK("soma(42,0)", g(0), 42);
        munmap(g, BUF);
    }

    /* 4 */
    printf("=== Teste 4: PARAM+PARAM -> soma(x,y) ===\n");
    {
        DescParam p[] = {{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,PARAM,{.v_int=0}}};
        cria_func(soma, p, 2, codigo);
        func2 g = aloca_exec(codigo, BUF);
        CHECK("soma(2,3)",   g(2,3),   5);
        CHECK("soma(-1,1)",  g(-1,1),  0);
        CHECK("soma(10,20)", g(10,20), 30);
        munmap(g, BUF);
    }

    /* 5 */
    printf("=== Teste 5: IND(INT)+FIX(INT) -> soma(*p,1) — muda *p entre chamadas ===\n");
    {
        int val = 99;
        DescParam p[] = {{INT_PAR,IND,{.v_ptr=&val}},{INT_PAR,FIX,{.v_int=1}}};
        cria_func(soma, p, 2, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("soma(*p=99,1)", g(), 100);
        val = 0;
        CHECK("soma(*p=0,1)",  g(),   1);
        munmap(g, BUF);
    }

    /* 6 */
    printf("=== Teste 6: FIX(PTR) -> escreve_ptr(&val) ===\n");
    {
        int val = 777;
        DescParam p[] = {{PTR_PAR,FIX,{.v_ptr=&val}}};
        cria_func(escreve_ptr, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("escreve_ptr(ptr fixo=777)", g(), 777);
        munmap(g, BUF);
    }

    /* 7 */
    printf("=== Teste 7: IND(PTR) -> escreve_ptr(**pp) ===\n");
    {
        int val = 555;
        void *pp = &val;
        DescParam p[] = {{PTR_PAR,IND,{.v_ptr=&pp}}};
        cria_func(escreve_ptr, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("escreve_ptr(**pp=555)", g(), 555);
        munmap(g, BUF);
    }

    /* 8 */
    printf("=== Teste 8: PARAM unico -> identidade(x) ===\n");
    {
        DescParam p[] = {{INT_PAR,PARAM,{.v_int=0}}};
        cria_func(identidade, p, 1, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("identidade(42)",  g(42),  42);
        CHECK("identidade(-1)",  g(-1),  -1);
        CHECK("identidade(0)",   g(0),    0);
        munmap(g, BUF);
    }

    /* 9 */
    printf("=== Teste 9: FIX(INT)x3 -> soma3(1,2,3) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=1}},{INT_PAR,FIX,{.v_int=2}},{INT_PAR,FIX,{.v_int=3}}};
        cria_func(soma3, p, 3, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("soma3(1,2,3)", g(), 6);
        munmap(g, BUF);
    }

    /* 10 */
    printf("=== Teste 10: PARAMx3 -> soma3(x,y,z) ===\n");
    {
        DescParam p[] = {{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,PARAM,{.v_int=0}}};
        cria_func(soma3, p, 3, codigo);
        func3 g = aloca_exec(codigo, BUF);
        CHECK("soma3(1,2,3)",    g(1,2,3),    6);
        CHECK("soma3(10,20,30)", g(10,20,30), 60);
        munmap(g, BUF);
    }

    /* 11 */
    printf("=== Teste 11: FIX+PARAM+IND(INT) -> soma3(10,x,*p) ===\n");
    {
        int val = 5;
        DescParam p[] = {{INT_PAR,FIX,{.v_int=10}},{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,IND,{.v_ptr=&val}}};
        cria_func(soma3, p, 3, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("soma3(10,3,*p=5)", g(3), 18);
        val = 0;
        CHECK("soma3(10,7,*p=0)", g(7), 17);
        munmap(g, BUF);
    }

    /* 12 */
    printf("=== Teste 12: FIX(INT)+FIX(INT) -> mul(6,7) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=6}},{INT_PAR,FIX,{.v_int=7}}};
        cria_func(mul, p, 2, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("mul(6,7)", g(), 42);
        munmap(g, BUF);
    }

    /* 13 — valor inteiro maximo */
    printf("=== Teste 13: FIX(INT) com INT_MAX -> identidade(INT_MAX) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=INT_MAX}}};
        cria_func(identidade, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("identidade(INT_MAX)", g(), INT_MAX);
        munmap(g, BUF);
    }

    /* 14 — valor inteiro minimo */
    printf("=== Teste 14: FIX(INT) com INT_MIN -> identidade(INT_MIN) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=INT_MIN}}};
        cria_func(identidade, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("identidade(INT_MIN)", g(), INT_MIN);
        munmap(g, BUF);
    }

    /* 15 — FIX zero */
    printf("=== Teste 15: FIX(INT) zero -> soma(0,0) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=0}},{INT_PAR,FIX,{.v_int=0}}};
        cria_func(soma, p, 2, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("soma(0,0)", g(), 0);
        munmap(g, BUF);
    }

    /* 16 — sub com PARAM invertido: sub(x, 10) */
    printf("=== Teste 16: PARAM+FIX -> sub(x,10) ===\n");
    {
        DescParam p[] = {{INT_PAR,PARAM,{.v_int=0}},{INT_PAR,FIX,{.v_int=10}}};
        cria_func(sub, p, 2, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("sub(15,10)", g(15),  5);
        CHECK("sub(0,10)",  g(0),  -10);
        munmap(g, BUF);
    }

    /* 17 — sub com FIX+PARAM: sub(100, x) */
    printf("=== Teste 17: FIX+PARAM -> sub(100,x) ===\n");
    {
        DescParam p[] = {{INT_PAR,FIX,{.v_int=100}},{INT_PAR,PARAM,{.v_int=0}}};
        cria_func(sub, p, 2, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("sub(100,30)", g(30), 70);
        CHECK("sub(100,100)", g(100), 0);
        munmap(g, BUF);
    }

    /* 18 — IND com valor negativo */
    printf("=== Teste 18: IND(INT) com valor negativo -> identidade(*p=-99) ===\n");
    {
        int val = -99;
        DescParam p[] = {{INT_PAR,IND,{.v_ptr=&val}}};
        cria_func(identidade, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("identidade(*p=-99)", g(), -99);
        munmap(g, BUF);
    }

    /* 19 — IND muda ponteiro base (pp aponta pra outra variavel) */
    printf("=== Teste 19: IND(PTR) muda pp para outro objeto ===\n");
    {
        int a = 111, b = 222;
        void *pp = &a;
        DescParam p[] = {{PTR_PAR,IND,{.v_ptr=&pp}}};
        cria_func(escreve_ptr, p, 1, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("escreve_ptr(**pp->a=111)", g(), 111);
        pp = &b;
        CHECK("escreve_ptr(**pp->b=222)", g(), 222);
        munmap(g, BUF);
    }

    /* 20 — PARAM+IND+FIX misturados (ordem diferente do teste 11) */
    printf("=== Teste 20: PARAM+IND(INT)+FIX -> soma3(x,*p,5) ===\n");
    {
        int val = 3;
        DescParam p[] = {
            {INT_PAR,PARAM,{.v_int=0}},
            {INT_PAR,IND,  {.v_ptr=&val}},
            {INT_PAR,FIX,  {.v_int=5}}
        };
        cria_func(soma3, p, 3, codigo);
        func1 g = aloca_exec(codigo, BUF);
        CHECK("soma3(10,*p=3,5)", g(10), 18);
        val = 7;
        CHECK("soma3(1,*p=7,5)",  g(1),  13);
        munmap(g, BUF);
    }

    /* 21 — funcao sem argumentos bindados (retorna constante) */
    printf("=== Teste 21: sem parametros -> retorna_zero() ===\n");
    {
        cria_func(retorna_zero, NULL, 0, codigo);
        func0 g = aloca_exec(codigo, BUF);
        CHECK("retorna_zero()", g(), 0);
        munmap(g, BUF);
    }

    printf("\n=== RESULTADO FINAL: %d ok, %d falhou ===\n", ok, falhou);
    return falhou ? 1 : 0;
}
