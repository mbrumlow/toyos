disk_load:
  push dx                              ; Save dx on the stack so we can
                                       ; remember how many sectors were
				       ; requested.

  mov ah, 0x02                         ; BIOS read sector function.
  mov al, dh                           ; Number of sectors to read.
  mov ch, 0x00                         ; Select cylinder 0.
  mov dh, 0x00                         ; Select head 0.
;;  mov cl, 0x02                         ; Starting sector.

  int 0x13                             ; BIOS interrupt.
  jc disk_error1                       ; Jump to disk_error if we had an error.

  pop dx                               ; Restore dx from stack.
  cmp dh, al                           ; Check that sector read returned the
                                       ; number of sectors requested.
  jne disk_error2                      ; Sectors requested was incorrect.
  ret

disk_error1:
  mov bx, DISK_ERROR_MSG1
  call print_boot_status
  jmp $

disk_error2:
  mov bx, DISK_ERROR_MSG2
  call print_boot_status
  jmp $

; Varables
DISK_ERROR_MSG1 db "Disk read error!", 0
DISK_ERROR_MSG2 db "Disk read error, wrong sector count!", 0

