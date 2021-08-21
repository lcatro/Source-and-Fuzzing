
#include <stdio.h>

#define MAX_SIZE (0x100)


typedef struct {
    char buffer[MAX_SIZE];
    int a;
    int b;
    int c;
    int d;
} data;


int main() {
    data test = {0};

    printf("no crash! A \n");

    test.buffer[0x1001] = 0xFF;

    printf("no crash! B \n");

    test.buffer[0x101] = 0xFF;

    printf("try crash! --> %X \n",sizeof(data));

    test.buffer[sizeof(data)] = 0xFF;

    return 0;
}

/*

ubuntu@ubuntu-virtual-machine:~/Desktop/vm_qemu/qemu_fuzzer/instrument$ ./example7 
no crash! A 
no crash! B 
try crash!
=================================================================
==82233==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffe696249f0 at pc 0x0000004c51cc bp 0x7ffe696248b0 sp 0x7ffe696248a8
WRITE of size 1 at 0x7ffe696249f0 thread T0
    #0 0x4c51cb in main (/home/ubuntu/Desktop/vm_qemu/qemu_fuzzer/instrument/example7+0x4c51cb)
    #1 0x7ff7823edcb1 in __libc_start_main csu/../csu/libc-start.c:314:16
    #2 0x41b2bd in _start (/home/ubuntu/Desktop/vm_qemu/qemu_fuzzer/instrument/example7+0x41b2bd)

Address 0x7ffe696249f0 is located in stack of thread T0 at offset 304 in frame
    #0 0x4c4f5f in main (/home/ubuntu/Desktop/vm_qemu/qemu_fuzzer/instrument/example7+0x4c4f5f)

  This frame has 1 object(s):
    [32, 304) 'test' <== Memory access at offset 304 overflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-overflow (/home/ubuntu/Desktop/vm_qemu/qemu_fuzzer/instrument/example7+0x4c51cb) in main
Shadow bytes around the buggy address:
  0x10004d2bc8e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc8f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc900: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc910: 00 00 00 00 00 00 00 00 f1 f1 f1 f1 00 00 00 00
  0x10004d2bc920: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x10004d2bc930: 00 00 00 00 00 00 00 00 00 00 00 00 00 00[f3]f3
  0x10004d2bc940: f3 f3 f3 f3 f3 f3 f3 f3 00 00 00 00 00 00 00 00
  0x10004d2bc950: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc960: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc970: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x10004d2bc980: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==82233==ABORTING
ubuntu@ubuntu-virtual-machine:~/Desktop/vm_qemu/qemu_fuzzer/instrument$ 

*/

