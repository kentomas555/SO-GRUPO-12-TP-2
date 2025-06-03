GLOBAL createProcessStackframe

start:
    call rcx
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
    
    push 0x10           ; R15
    push 0x0F           ; R14
    push 0x0E           ; R13
    push 0x0D           ; R12
    push 0x0C           ; R11
    push 0x0B           ; R10
    push 0x0A           ; R9
    push 0x09           ; R8
    push rsi            ; RSI = argv
    push rdi            ; RDI = argc
    push rdx            ; RDX = stack
    push rcx            ; RCX = program
    push 0x02           ; RBX
    push 0x01           ; RAX  

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret 