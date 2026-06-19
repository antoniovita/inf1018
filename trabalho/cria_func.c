/* Antonio Vita Fonseca 2510071 3WC */
/* David Faerman 2510179 3WC */

#include "cria_func.h"

static void emit_bytes(unsigned char *buf, int *pos, unsigned long val, int n) {
    for (int i = 0; i < n; i++) {
        buf[(*pos)++] = val & 0xFF;
        val >>= 8;
    }
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]) {
    int pos = 0;

    /*
     * Opcodes de push/pop para cada registrador de argumento:
     * índice 0 → %rdi, 1 → %rsi, 2 → %rdx
     */
    unsigned char push_regs[] = {0x57, 0x56, 0x52}; /* push %rdi, %rsi, %rdx */
    unsigned char pop_regs[]  = {0x5F, 0x5E, 0x5A}; /* pop  %rdi, %rsi, %rdx */

    /* ── PRÓLOGO ─────────────────────────────────────────────── */
    codigo[pos++] = 0x55;        /* push %rbp          */
    codigo[pos++] = 0x48;        /* \                  */
    codigo[pos++] = 0x89;        /*  > mov %rsp, %rbp  */
    codigo[pos++] = 0xE5;        /* /                  */

    /* ── FASE 1: salvar na pilha os PARAMs recebidos ─────────── */
    /*
     * A nova função recebe seus argumentos em %rdi, %rsi, %rdx
     * (nessa ordem, um por PARAM encontrado).
     * Salvamos todos antes de sobrescrever os registradores.
     */
    int param_count = 0;
    for (int i = 0; i < n; i++) {
        if (params[i].orig_val == PARAM) {
            codigo[pos++] = push_regs[param_count];
            param_count++;
        }
    }

    /* ── FASE 2: montar argumentos para f (de trás pra frente) ── */
    /*
     * Percorremos params[] de trás pra frente para consumir a pilha
     * na ordem correta (LIFO): o último PARAM empilhado sai primeiro
     * e vai pro último registrador de argumento que precisar de PARAM.
     */
    for (int i = n - 1; i >= 0; i--) {

        if (params[i].orig_val == PARAM) {
            /* pop direto pro registrador de destino (posição i) */
            codigo[pos++] = pop_regs[i];
        }

        else if (params[i].orig_val == FIX) {

            if (params[i].tipo_val == INT_PAR) {
                /*
                 * mov $valor, %edi/%esi/%edx
                 * opcodes: BF / BE / BA
                 */
                unsigned char opcodes[] = {0xBF, 0xBE, 0xBA};
                codigo[pos++] = opcodes[i];
                emit_bytes(codigo, &pos, (unsigned int)params[i].valor.v_int, 4);
            }
            else { /* PTR_PAR */
                /*
                 * movabs $ptr, %rdi/%rsi/%rdx
                 * opcodes: 48 BF / 48 BE / 48 BA
                 */
                unsigned char opcodes[][2] = {{0x48,0xBF},{0x48,0xBE},{0x48,0xBA}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
                emit_bytes(codigo, &pos, (unsigned long)params[i].valor.v_ptr, 8);
            }
        }

        else { /* IND */
            /*
             * Primeiro carrega o endereço da variável em %rax:
             *   movabs $addr, %rax   →   48 B8 [8 bytes]
             * Depois lê o valor desse endereço pro registrador destino.
             */
            codigo[pos++] = 0x48;
            codigo[pos++] = 0xB8;
            emit_bytes(codigo, &pos, (unsigned long)params[i].valor.v_ptr, 8);

            if (params[i].tipo_val == INT_PAR) {
                /*
                 * mov (%rax), %edi/%esi/%edx
                 * opcodes: 8B 38 / 8B 30 / 8B 10
                 */
                unsigned char opcodes[][2] = {{0x8B,0x38},{0x8B,0x30},{0x8B,0x10}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
            }
            else { /* PTR_PAR */
                /*
                 * mov (%rax), %rdi/%rsi/%rdx
                 * opcodes: 48 8B 38 / 48 8B 30 / 48 8B 10
                 */
                unsigned char opcodes[][3] = {{0x48,0x8B,0x38},{0x48,0x8B,0x30},{0x48,0x8B,0x10}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
                codigo[pos++] = opcodes[i][2];
            }
        }
    }

    /* ── CHAMAR f ────────────────────────────────────────────── */
    /*
     * movabs $f, %rax   →   48 B8 [8 bytes]
     * call *%rax        →   FF D0
     * O retorno de f já fica em %rax e é repassado automaticamente.
     */
    codigo[pos++] = 0x48;
    codigo[pos++] = 0xB8;
    emit8(codigo, &pos, (unsigned long)f);
    codigo[pos++] = 0xFF;
    codigo[pos++] = 0xD0;

    /* ── EPÍLOGO ─────────────────────────────────────────────── */
    codigo[pos++] = 0xC9;  /* leave */
    codigo[pos++] = 0xC3;  /* ret   */
}