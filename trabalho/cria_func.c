/* Antonio Vita Fonseca 2510071 3WC */
/* David Faerman 2510179 3WC */

#include "cria_func.h"

static void emit_bytes(unsigned char *buf, int *pos, unsigned long val, int n) {
    for (int i = 0; i < n; i++) {
        buf[(*pos)++] = val;
        val >>= 8;
    }
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]) {
    int pos = 0;

    unsigned char push_regs[] = {0x57, 0x56, 0x52};
    unsigned char pop_regs[]  = {0x5F, 0x5E, 0x5A};

    codigo[pos++] = 0x55;
    codigo[pos++] = 0x48;
    codigo[pos++] = 0x89;
    codigo[pos++] = 0xE5;

    int param_count = 0;
    for (int i = 0; i < n; i++) {
        if (params[i].orig_val == PARAM) {
            codigo[pos++] = push_regs[param_count];
            param_count++;
        }
    }

    for (int i = n - 1; i >= 0; i--) {

        if (params[i].orig_val == PARAM) {
            codigo[pos++] = pop_regs[i];
        }

        else if (params[i].orig_val == FIX) {

            if (params[i].tipo_val == INT_PAR) {
                unsigned char opcodes[] = {0xBF, 0xBE, 0xBA};
                codigo[pos++] = opcodes[i];
                emit_bytes(codigo, &pos, (unsigned int)params[i].valor.v_int, 4);
            }

            else { 
                unsigned char opcodes[][2] = {{0x48,0xBF},{0x48,0xBE},{0x48,0xBA}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
                emit_bytes(codigo, &pos, (unsigned long)params[i].valor.v_ptr, 8);
            }
        }

        else {
            codigo[pos++] = 0x48;
            codigo[pos++] = 0xB8;
            emit_bytes(codigo, &pos, (unsigned long)params[i].valor.v_ptr, 8);

            if (params[i].tipo_val == INT_PAR) {
                unsigned char opcodes[][2] = {{0x8B,0x38},{0x8B,0x30},{0x8B,0x10}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
            }

            else {
                unsigned char opcodes[][3] = {{0x48,0x8B,0x38},{0x48,0x8B,0x30},{0x48,0x8B,0x10}};
                codigo[pos++] = opcodes[i][0];
                codigo[pos++] = opcodes[i][1];
                codigo[pos++] = opcodes[i][2];
            }
        }
    }

    codigo[pos++] = 0x48;
    codigo[pos++] = 0xB8;
    emit_bytes(codigo, &pos, (unsigned long)f, 8);
    codigo[pos++] = 0xFF;
    codigo[pos++] = 0xD0;

    codigo[pos++] = 0xC9;
    codigo[pos++] = 0xC3;
}