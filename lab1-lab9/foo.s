.text
.globl foo
.type foo, @function
foo:
    pushq %r13
    pushq %r12

    movl $0, %r10d
    movl $0, %r11d

L1:
    cmpl %esi, %r10d
    jge  FIM

    movl %r10d, %eax            
    imull $4, %eax              

    movq %rdi, %r13             
    addq %rax, %r13

    movl (%r13), %r12d
    addl %r12d, %r11d

    cmpl $0, %r12d
    jne  L3

L2:
    movl %r11d, (%r13)
    movl $0, %r11d

L3:
    addl $1, %r10d
    jmp  L1

FIM:
    popq %r12
    popq %r13
    ret



void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}