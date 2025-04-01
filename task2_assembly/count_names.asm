; Simple name counter for Linux x86-64
section .data
    filename db "names.txt", 0
    msg db "Number of names: "
    msg_len equ $ - msg
    newline db 10

section .bss
    buffer resb 4096
    num_str resb 16

section .text
    global _start

_start:
    ; Open file
    mov rax, 2           ; sys_open
    mov rdi, filename    ; filename
    mov rsi, 0           ; O_RDONLY
    mov rdx, 0           ; mode
    syscall
    
    cmp rax, 0
    jl exit_error
    
    ; Save file descriptor
    mov r12, rax
    
    ; Read file
    mov rax, 0           ; sys_read
    mov rdi, r12         ; fd
    mov rsi, buffer      ; buffer
    mov rdx, 4096        ; count
    syscall
    
    cmp rax, 0
    jl exit_error
    
    ; Save bytes read
    mov r13, rax
    
    ; Close file
    mov rax, 3           ; sys_close
    mov rdi, r12         ; fd
    syscall
    
    ; Count newlines
    xor r14, r14         ; counter = 0
    xor rcx, rcx         ; index = 0
    
count_loop:
    cmp rcx, r13
    jge count_done
    
    cmp byte [buffer + rcx], 10  ; newline?
    jne not_newline
    inc r14                      ; count++
    
not_newline:
    inc rcx
    jmp count_loop
    
count_done:
    ; Check if file ends with newline
    dec rcx
    cmp byte [buffer + rcx], 10
    je print_result
    inc r14                     ; Add 1 if no final newline
    
print_result:
    ; Print message
    mov rax, 1                  ; sys_write
    mov rdi, 1                  ; stdout
    mov rsi, msg                ; buffer
    mov rdx, msg_len            ; count
    syscall
    
    ; Convert number to string
    mov rax, r14                ; number to convert
    mov rbx, 10                 ; base 10
    mov rcx, num_str
    add rcx, 15                 ; end of buffer
    mov byte [rcx], 0           ; null terminator
    
convert_loop:
    dec rcx
    xor rdx, rdx                ; clear remainder
    div rbx                     ; divide by 10
    add rdx, '0'                ; convert to ASCII
    mov [rcx], dl               ; store digit
    test rax, rax               ; check if done
    jnz convert_loop
    
    ; Print number
    mov rax, 1                  ; sys_write
    mov rdi, 1                  ; stdout
    mov rsi, rcx                ; start of string
    mov rdx, num_str
    add rdx, 15
    sub rdx, rcx                ; calculate length
    syscall
    
    ; Print newline
    mov rax, 1                  ; sys_write
    mov rdi, 1                  ; stdout
    mov rsi, newline            ; buffer
    mov rdx, 1                  ; count
    syscall
    
    ; Exit success
    mov rax, 60                 ; sys_exit
    xor rdi, rdi                ; status 0
    syscall
    
exit_error:
    mov rax, 60                 ; sys_exit
    mov rdi, 1                  ; status 1
    syscall
