GLOBAL createProcessStackframe

start:
    call rcx        ; Llama al main del programa del proceso
    mov rax, 20   ; syscall 20
    int 80h         ; Simula syscall exit(0)

createProcessStackframe:
    push rbp
    mov rbp, rsp

    mov rsp, rdx    ; rdx = stack top del nuevo proceso

    push 0x0        ; SS
    push rdx        ; RSP
    push 0x202      ; RFLAGS 
    push 0x8        ; CS 
    push start      ; RIP (entry point del proceso)

    push 0x01       ; RAX
    push 0x02       ; RBX
    push rcx        ; RCX (dirección del main del programa)
    push 0x00       ; RDX (stack del proceso, opcional)
    push rdx        ; RBP
    push rdi        ; RDI (argc)
    push rsi        ; RSI (argv)
    push 0x09       ; R8
    push 0x0A       ; R9
    push 0x0B       ; R10
    push 0x0C       ; R11
    push 0x0D       ; R12
    push 0x0E       ; R13
    push 0x0F       ; R14
    push 0x10       ; R15

    mov rax, rsp    ; rax = nuevo stack pointer (RSP) para el PCB

    mov rsp, rbp
    pop rbp
    ret