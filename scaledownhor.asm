section .text
global scaledownhor

; void scaledownhor(void* src, void* dst, uint32_t width, 
;                      uint32_t height, uint32_t input_stride, 
;                      uint32_t output_stride, uint32_t scale);
scaledownhor:
    push rbp
    mov rbp, rsp
    sub rsp, 64         ; Allocate space
    
    ; Save arguments
    mov [rbp-8], rdi    ; src
    mov [rbp-16], rsi   ; dst
    mov [rbp-20], edx   ; width
    mov [rbp-24], ecx   ; height
    mov [rbp-28], r8d   ; input_stride
    mov [rbp-36], r9d   ; output_stride
    mov eax, [rbp+16]   ; scale
    mov [rbp-40], eax

    ; Calculate new_width = width / scale
    mov eax, [rbp-20]
    xor edx, edx
    div dword [rbp-40]
    mov [rbp-44], eax   ; new_width

    ; Reset y
    mov dword [rbp-48], 0

.row_loop:
    mov eax, [rbp-48]
    cmp eax, [rbp-24]
    jge .done

    ; Calculate row offset
    mov eax, [rbp-48]
    mul dword [rbp-28]
    mov r10, [rbp-8]
    add r10, rax        ; r10 = current row src

    mov eax, [rbp-48]
    mul dword [rbp-36]
    mov r11, [rbp-16]
    add r11, rax        ; r11 = current row dst

    ; Reset column counter (x_new)
    mov dword [rbp-52], 0

.pixel_loop:
    mov eax, [rbp-52]
    cmp eax, [rbp-44]
    jge .next_row

    ; Calculate x_original = x_new * scale
    mov eax, [rbp-52]
    mul dword [rbp-40]
    mov [rbp-56], eax   ; x_original

    ; Initialise maximums
    mov byte [rbp-57], 0 ; max B
    mov byte [rbp-58], 0 ; max G
    mov byte [rbp-59], 0 ; max R

    ; Counter in group (i)
    mov dword [rbp-64], 0

.group_loop:
    mov eax, [rbp-64]
    cmp eax, [rbp-40]   ; i < scale?
    jge .store_max

    ; Check if not outside of the image
    mov eax, [rbp-56]   ; x_original
    add eax, [rbp-64]   ; + i
    cmp eax, [rbp-20]   ; width
    jge .store_max

    ; Calculate pixel offset
    imul eax, 3
    mov rsi, r10
    add rsi, rax        ; src + offset

    ; Update maximums (temporary register - al)
    mov al, [rsi]       ; B
    cmp al, [rbp-57]
    jbe .check_green
    mov [rbp-57], al    ; new max B

.check_green:
    mov al, [rsi+1]     ; G
    cmp al, [rbp-58]
    jbe .check_red
    mov [rbp-58], al    ; new max G

.check_red:
    mov al, [rsi+2]     ; R
    cmp al, [rbp-59]
    jbe .next_in_group
    mov [rbp-59], al    ; new max R

.next_in_group:
    inc dword [rbp-64]
    jmp .group_loop

.store_max:
    ; Calculate desination position
    mov eax, [rbp-52]
    imul eax, 3
    mov rdi, r11
    add rdi, rax        ; dst + offset

    ; Save maximums
    mov al, [rbp-57]
    mov [rdi], al       ; B
    mov al, [rbp-58]
    mov [rdi+1], al     ; G
    mov al, [rbp-59]
    mov [rdi+2], al     ; R

    ; Increment x_new
    inc dword [rbp-52]
    jmp .pixel_loop

.next_row:
    inc dword [rbp-48]
    jmp .row_loop

.done:
    add rsp, 64
    pop rbp
    ret