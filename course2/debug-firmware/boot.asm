; boot.asm - 显示 "Hello, world!" 的引导扇区
org 0x7c00                ; BIOS 会把引导扇区加载到 0x7c00

start:
    ; 设置段寄存器
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; 显示字符串
    mov si, message

print_loop:
    lodsb                  ; 加载字符串中的一个字符到 AL
    cmp al, 0
    je hang                ; 如果是 0（字符串结束），就跳转到 hang
    mov ah, 0x0e           ; BIOS teletype function
    int 0x10               ; 调用 BIOS 中断打印字符
    jmp print_loop

hang:
    jmp $

message db 'Hello, world!', 0

times 510-($-$$) db 0      ; 填充到 510 字节
dw 0xaa55                  ; 启动扇区魔数
