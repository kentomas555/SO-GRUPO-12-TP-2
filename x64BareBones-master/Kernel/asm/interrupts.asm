
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irs80Handler

GLOBAL _exception0Handler
GLOBAL _exception1Handler

GLOBAL getAllRegisters

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN getStackBase

EXTERN saveRegs

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	; llamo para capturar acá los registros *****
	captureRegistersMacro

	mov rdi, %1 ; pasaje de parametro

	call exceptionDispatcher

	call getStackBase
	mov [rsp+24], rax
	mov qword [rsp], 0x400000
	iretq
%endmacro


%macro captureRegistersMacro 0:
	
	mov[registros], rax
    mov[registros + 8], rbx
    mov[registros + 8*2], rcx
    mov[registros + 8*3], rdx
    mov[registros + 8*4], rsi
    mov[registros + 8*5], rdi

    mov[registros + 8*6], r8
    mov[registros + 8*7], r9
    mov[registros + 8*8], r10
    mov[registros + 8*9], r11
    mov[registros + 8*10], r12
    mov[registros + 8*11], r13
    mov[registros + 8*12], r14
    mov[registros + 8*13], r15

	mov rax, [rsp]
	mov[registros + 8*14], rax ;RIP

	mov rax, [rsp + 8]
	mov[registros + 8*15], rax ;CS

	mov rax, [rsp + 8*2]
	mov[registros + 8*16], rax ;RFLAGS

	mov rax, [rsp + 8*3]
	mov[registros + 8*17], rax ;RSP

	mov rax, [rsp + 8*4]
	mov[registros + 8*18], rax ;SS

	mov[registros + 8*19], rbp ;RBP

%endmacro

getAllRegisters:
	push rbp
	mov rbp, rsp

	mov rax, registros

	mov rsp, rbp
	pop rbp

	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Software Interrupts
_irs80Handler:
	captureRegistersMacro
	
	push rbx

	call syscallDispatcher

	pop rbx

	iretq


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception1Handler:
	exceptionHandler 6


haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	regs resq 144
	registros resq 144