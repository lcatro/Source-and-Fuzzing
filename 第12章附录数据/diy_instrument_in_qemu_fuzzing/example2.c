

#include <stdio.h>


void try_write(int a) {
    printf("try write !!!\n");

    if (a==1) {
        ;
    } else {
        ;
    }
}


int try_read(int b) {
    printf("try read !!!\n");

    if (b==1) {
        ;
    } else {
        ;
    }

    return 0;
}


int main(int argc,char** argv) {
    __sanitizer_enter();
    printf("main running !!!\n");

    try_write(1);
    try_read(2);

    printf("main exit !!\n");

    __sanitizer_exit();
    return 1;
}



