.text

prologo:
    push %rbp
    mov  %rsp, %rbp

# FASE 1: salvar PARAMs recebidos
salvar_params:
    push %rdi
    push %rsi
    push %rdx

# FASE 2: recuperar PARAMs (pop pros registradores destino)
pop_params:
    pop %rdi
    pop %rsi
    pop %rdx

# FIX com INT (valor de 32 bits) pra cada registrador
fix_int:
    mov $10, %edi
    mov $10, %esi
    mov $10, %edx

# FIX com PTR (endereço de 64 bits) pra cada registrador
fix_ptr:
    movabs $0x1122334455667788, %rdi
    movabs $0x1122334455667788, %rsi
    movabs $0x1122334455667788, %rdx

# IND com INT (lê inteiro de um endereço)
ind_int:
    movabs $0x1122334455667788, %rax
    mov (%rax), %edi
    movabs $0x1122334455667788, %rax
    mov (%rax), %esi
    movabs $0x1122334455667788, %rax
    mov (%rax), %edx

# IND com PTR (lê ponteiro de um endereço)
ind_ptr:
    movabs $0x1122334455667788, %rax
    mov (%rax), %rdi
    movabs $0x1122334455667788, %rax
    mov (%rax), %rsi
    movabs $0x1122334455667788, %rax
    mov (%rax), %rdx

# Chamar f
call_f:
    movabs $0x1122334455667788, %rax
    call *%rax

# Epílogo
epi:
    leave
    ret