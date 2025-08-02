# Registers

| Register | Hex Value | Decimal Value |
|----------|-----------|---------------|
| rax | 0x0000000000000000 | 0 |
| rbx | 0x0000000000400518 | 4195608 |
| rcx | 0x000000000045217b | 4530555 |
| rdx | 0x00000000000d7160 | 880992 |
| rsi | 0x00000000000d7160 | 880992 |
| rdi | 0x00007ffff7f20000 | 140737353220096 |
| rbp | 0x00007fffffffde70 | 140737488346736 |
| rsp | 0x00007fffffffdda0 | 140737488346528 |
| r8 | 0x000000000000000a | 10 |
| r9 | 0x0000000000000003 | 3 |
| r10 | 0x0000000000000001 | 1 |
| r11 | 0x0000000000000217 | 535 |
| r12 | 0x0000000000402ff0 | 4206576 |
| r13 | 0x0000000000000000 | 0 |
| r14 | 0x00000000004c1018 | 4984856 |
| r15 | 0x0000000000000000 | 0 |
| rip | 0x0000000000401f46 | 4202310 |
| eflags | 0x0000000000000217 | 535 |

# Memory Mappings

| Start Address | End Address | Size | Permissions | Name |
|---------------|-------------|------|--------------|------|
| 0x400000 | 0x401000 | 0x1000 | /mnt/c/study/Mlsys/OS-jyy/course3/address-space/mmap-demo |  |
| 0x401000 | 0x496000 | 0x95000 | /mnt/c/study/Mlsys/OS-jyy/course3/address-space/mmap-demo |  |
| 0x496000 | 0x4bd000 | 0x27000 | /mnt/c/study/Mlsys/OS-jyy/course3/address-space/mmap-demo |  |
| 0x4be000 | 0x4c1000 | 0x3000 | /mnt/c/study/Mlsys/OS-jyy/course3/address-space/mmap-demo |  |
| 0x4c1000 | 0x4c4000 | 0x3000 | /mnt/c/study/Mlsys/OS-jyy/course3/address-space/mmap-demo |  |
| 0x4c4000 | 0x4c5000 | 0x1000 | [heap] |  |
| 0x4c5000 | 0x4e8000 | 0x23000 | [heap] |  |
| 0x7ffff7ff9000 | 0x7ffff7ffd000 | 0x4000 | [vvar] |  |
| 0x7ffff7ffd000 | 0x7ffff7fff000 | 0x2000 | [vdso] |  |
| 0x7ffffffde000 | 0x7ffffffff000 | 0x21000 | [stack] |  |
