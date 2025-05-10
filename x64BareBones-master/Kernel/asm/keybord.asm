section .text 

    GLOBAL getKey

getKey:

    push rbp
    mov rbp, rsp

    mov eax, 0

    mov al, 60h
    out 64h, al
    in ax, 60h

    mov rsp, rbp
    pop rbp
    ret 