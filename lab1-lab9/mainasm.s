.data                              
nums: .int 3, -5, 7, 8, -2         
s1:   .string "%d\n"               

.text                              
.globl main                        
main:
  /* prologo */
     pushq %rbp                      
     movq  %rsp, %rbp                
     subq  $16, %rsp                 
     movq  %rbx, -8(%rbp)            
     movq  %r12, -16(%rbp)           

     movq $0, %rbx 
     leaq  nums(%rip), %r12          

L1:
     cmpq  $5, %rbx
     je    L2    

     movl  (%r12), %edi              # 1º arg filtro: x = *p   (int em %edi)
     movl  $1, %esi                  # 2º arg filtro: lim = 1  (int em %esi)
     call  filtro                    # eax = filtro(x,lim)

     leaq  s1(%rip), %rdi            # 1º arg printf: format "%d\n"
     movl  %eax, %esi                # 2º arg printf: valor (retorno da filtro)
     xorl  %eax, %eax                # printf variádica: %al = 0
     call  printf                    # imprime

     addq  $4, %r12                  # p++ (avança 4 bytes = 1 int)
     incq  %rbx                      # i++
     jmp   L1                        # volta pro laço

L2:
     movl  $0, %eax                  # return 0

  /* finalizacao */
     movq -8(%rbp), %rbx             # restaura rbx
     movq -16(%rbp), %r12            # restaura r12
     leave                           # desfaz frame
     ret                             # retorna