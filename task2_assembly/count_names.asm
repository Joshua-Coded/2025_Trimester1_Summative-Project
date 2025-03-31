; count_names.asm - Assembly program to count names in a file
; For Linux x86-64 systems
; Reads names.txt and outputs the count of names to the console

section .data
    filename db "names.txt", 0          ; Filename with null terminator
    msg db "Number of names: "          ; Output message prefix
    msg_len equ $ - msg                 ; Length of message
    newline db 10                       ; Newline character (ASCII 10)
    error_msg db "Error: Could not open or read file", 10
    error_msg_len equ $ - error_msg     ; Length of error message

section .bss
    buffer resb 4096                    ; Buffer for file content (4KB)
    count_str resb 20                   ; Buffer for count as string

section .text
    global _start

_start:
    ; Open the file
    mov rax, 2                          ; syscall: open
    mov rdi, filename                   ; filename
    mov rsi, 0                          ; flags: O_RDONLY
    mov rdx, 0                          ; mode (not used for reading)
    syscall

    ; Check for error (negative return value)
    cmp rax, 0
    jl open_error                       ; Jump if less than 0 (error)

    ; Save file descriptor
    mov r8, rax

    ; Read from file
    mov rax, 0                          ; syscall: read
    mov rdi, r8                         ; file descriptor
    mov rsi, buffer                     ; buffer address
    mov rdx, 4096                       ; buffer size
    syscall

    ; Check for read error
    cmp rax, 0
    jl read_error

    ; Save number of bytes read
    mov r9, rax

    ; Close the file
    mov rax, 3                          ; syscall: close
    mov rdi, r8                         ; file descriptor
    syscall

    ; Count names (count newlines)
    mov r10, 0                          ; Initialize counter
    mov rcx, 0                          ; Initialize index

count_loop:
    cmp rcx, r9                         ; Compare index with bytes read
    jge count_done                      ; If we've read all bytes, finish counting

    mov al, [buffer + rcx]              ; Get current byte
    inc rcx                             ; Move to next byte

    cmp al, 10                          ; Is it a newline?
    jne count_loop                      ; If not, continue loop
    inc r10                             ; If newline, increment counter
    jmp count_loop                      ; Continue loop

count_done:
    ; Check if the last character is not a newline
    ; If file doesn't end with newline, add 1 to count for the last name
    cmp r9, 0                           ; Check if file is empty
    je print_count                      ; Skip if empty file
    
    mov al, [buffer + r9 - 1]           ; Get last byte
    cmp al, 10                          ; Is it a newline?
    je print_count                      ; If yes, count is correct
    inc r10                             ; If not, increment count for last name

print_count:
    ; Convert count to string
    mov rax, r10                        ; Number to convert
    mov rcx, 10                         ; Base 10
    mov rdi, count_str                  ; String buffer
    add rdi, 19                         ; Point to end of buffer
    mov byte [rdi], 0                   ; Null terminator
    dec rdi                             ; Move back one position

convert_loop:
    xor rdx, rdx                        ; Clear rdx for division
    div rcx                             ; Divide rax by 10, remainder in rdx
    add dl, '0'                         ; Convert to ASCII
    mov [rdi], dl                       ; Store digit
    dec rdi                             ; Move to next position left
    test rax, rax                       ; Is quotient zero?
    jnz convert_loop                    ; If not, continue conversion
    
    inc rdi                             ; Point to first digit

    ; Print message
    mov rax, 1                          ; syscall: write
    mov rdi, 1                          ; file descriptor: stdout
    mov rsi, msg                        ; message buffer
    mov rdx, msg_len                    ; message length
    syscall

    ; Print count
    mov rsi, rdi                        ; Start of count string
    mov rdx, count_str                  ; End of buffer
    add rdx, 19                         ; Point to last byte
    sub rdx, rsi                        ; Calculate length
    mov rax, 1                          ; syscall: write
    mov rdi, 1                          ; file descriptor: stdout
    syscall

    ; Print newline
    mov rax, 1                          ; syscall: write
    mov rdi, 1                          ; file descriptor: stdout
    mov rsi, newline                    ; newline character
    mov rdx, 1                          ; length: 1 byte
    syscall

    ; Exit successfully
    mov rax, 60                         ; syscall: exit
    xor rdi, rdi                        ; status: 0 (success)
    syscall

open_error:
read_error:
    ; Print error message
    mov rax, 1                          ; syscall: write
    mov rdi, 1                          ; file descriptor: stderr
    mov rsi, error_msg                  ; error message
    mov rdx, error_msg_len              ; message length
    syscall

    ; Exit with error
    mov rax, 60                         ; syscall: exit
    mov rdi, 1                          ; status: 1 (error)
    syscall