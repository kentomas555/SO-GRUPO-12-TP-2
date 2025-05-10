GLOBAL syscall
GLOBAL throw_invalid_opcode
GLOBAL throw_zero_division

SECTION .text

syscall:

    push rbx              ; Preserva el registro rbx
    int 80h               ; Llama a la interrupción 0x80
    pop rbx               ; Restaura el valor original de rbx
    ret                   ; Retorna al llamador
    
throw_zero_division:
    mov rax, 0
    div rax
    ret

throw_invalid_opcode:
    ud2
    ret
