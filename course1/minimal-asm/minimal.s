# 1 "minimal.S"
# 1 "/mnt/sda1/wch/study/mlsys/OS-jyy/course1/minimal-asm//"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/local/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "minimal.S"
# 1 "/usr/local/include/sys/syscall.h" 1 3
# 24 "/usr/local/include/sys/syscall.h" 3
# 1 "/usr/include/asm/unistd.h" 1 3 4
# 12 "/usr/include/asm/unistd.h" 3 4
# 1 "/usr/include/asm/unistd_64.h" 1 3 4
# 13 "/usr/include/asm/unistd.h" 2 3 4
# 25 "/usr/local/include/sys/syscall.h" 2 3






# 1 "/usr/local/include/bits/syscall.h" 1 3
# 32 "/usr/local/include/sys/syscall.h" 2 3
# 2 "minimal.S" 2
# 19 "minimal.S"
.globl _start
_start:
    movq $1, %rax; movq $1, %rdi; movq $addr1, %rsi; movq $addr2 - addr1, %rdx; syscall
    movq $60, %rax; movq $1, %rdi; movq $0, %rsi; movq $0, %rdx; syscall

addr1:
    .ascii "\033[01;31mHello, OS World\033[0m\n"
addr2:
