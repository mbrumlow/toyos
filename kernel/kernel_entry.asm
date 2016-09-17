global start

section .text

bits 32
extern main

mov esp, stack_top                     ; Setup stack.

call init_pd
call main
jmp $

init_pd:

  ; Target system only has ~6 megabytes of ram. For now we will have 8 megabytes
  ; addressable vm.

  mov eax, 0x0
  mov ebx, page_table
  mov edx, 0x3
  .fill_pd:
    mov ecx, ebx
    or ecx, edx
    mov [page_directory+eax*4], ecx
    add ebx, 4096
    inc eax
    cmp eax, 2
    jne .next_pd_entry
    mov edx, 0x0
    .next_pd_entry:
    cmp eax, 1024
    je .fill_pd_end
    jmp .fill_pd
  .fill_pd_end:


  ; Identity map the first ~4MB for now.

  mov eax, 0x0
  mov ebx, 0x0
  mov edx, 0x3
  .fill_table:
    mov ecx, ebx
    or ecx, edx
    mov [page_table+eax*4], ecx
    add ebx, 4096
    inc eax
    cmp eax, 1024
    jne .fill_table_next
    mov edx, 0x0
    .fill_table_next:
    cmp eax, 2048
    je .fill_table_end
    jmp .fill_table
  .fill_table_end:

  mov eax, page_directory
  mov cr3, eax

  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax

  ret

section .bss
align 4096
page_directory:
 resb 4096
page_table:
 resb 4096
 resb 4096
stac_bottom:
 resb 1024
stack_top:

