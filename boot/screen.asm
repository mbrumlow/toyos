
init_screen:
  call clear_screen
  call reset_cursor
  ret

clear_screen:
  mov ah, 0x06
  mov al, 0x00
  mov bh, 0x07
  mov ch, 0x00
  mov cl, 0x00
  mov dh, 0x18
  mov dl, 0x4f
  int 0x10
  ret

reset_cursor:
  mov ah, 0x02
  mov bh, 0
  mov dh, 0x00
  mov dl, 0x00
  int 0x10
  ret


