GLOBAL createProcessStackframe

start:
    call rdx
    mov rax, 0x42
    int 80h

createProcessStackframe:
    push rbp
    mov rbp, rsp
    
    mov rsp, rdx    ;rdx = CurrentStack
    push 0x0
    push rdx        ;push RSP = CurrentStack
    push 0x202
    push 0x8
    push start   
    
    push 0x01
    push 0x02
    push rcx    ;rcx = program
    push rdx    ;rdx = CurrentStack
	push rdi    ;rdi = argc
	push rsi    ;rsi = argv
	push 0x08
	push 0x09
	push 0x0A
	push 0x0B
  	push 0x0C
	push 0x0D
	push 0x0E
	push 0x0F   

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    iretq 