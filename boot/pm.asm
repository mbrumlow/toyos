[bits 16]

; Switch to protected mode
switch_to_pm:

  cli                    ; Turn inttrupts off

  lgdt [gdt_descriptor]  ; Load gdt

  mov eax, cr0           ; Move to protected mode by setting the first bit of
  or eax, 0x1            ; CR0 (control register)
  mov cr0, eax

  jmp CODE_SEG:init_pm   ; Make far jump to our 32bit code
                         ; Forces flush of cpu cache


[bits 32]

; Init registers and stack in PM
init_pm:

  mov ax, DATA_SEG       ; In PM our old segments are meaningliss, so we have
  mov ds, ax             ; point our segmentn register to the data selector we
  mov ss, ax             ; defined in our gdt.
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x1000       ; Update our stack pointer so it is at the top of the
  mov esp, ebp           ; free spaec.

  call BEGIN_PM

