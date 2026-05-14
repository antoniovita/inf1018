.text
.globl fat
.type fat, @function
fat:
    cmpl $0, %edi
    je L1

    pushq %rdi
    subl $1, %edi
    call fat
    popq %rcx
    imull %ecx, %eax
    ret

L1:
    movl $1, %eax
    ret