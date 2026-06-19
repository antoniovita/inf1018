.text
.globl foo
foo:
    movl %edi, %eax
    addl $1, %eax
    ret