.global _start
.section .data
.section .text
_start:
    # Your generated code goes here
    movq $60, %rax
    xorq %rdi, %rdi
    syscall
