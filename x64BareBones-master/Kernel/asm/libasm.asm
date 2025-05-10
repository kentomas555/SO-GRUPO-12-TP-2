GLOBAL cpuVendor
GLOBAL outb
GLOBAL inb
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
GLOBAL getDays
GLOBAL getMonth
GLOBAL getYear

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

outb:
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	ret

inb:
	mov rdx, rdi
	in al, dx
	ret

getSeconds:
    cli
    call toBinary

    mov al, 0x00
    out 70h, al
    in al, 71h
    sti

    ret

getMinutes:
	cli 
	call toBinary

	mov al, 0x02
    out 70h, al
    in al, 71h
    sti

    ret

getHours:
	cli 
	call toBinary
	
	
	mov al, 0x04
    out 70h, al
    in al, 71h
    sti

    ret

getDays:
	cli 
	call toBinary

	mov al, 0x07
    out 70h, al
    in al, 71h
    sti

	ret

getMonth:
	cli 
	call toBinary

	mov al, 0x08
    out 70h, al
    in al, 71h
    sti

	ret

getYear:
	cli 
	call toBinary

	mov al, 0x09
    out 70h, al
    in al, 71h
    sti

	ret

toBinary:
	mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al
	ret
