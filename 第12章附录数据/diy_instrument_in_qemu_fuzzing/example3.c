

#include <stdio.h>



int foo(int b) {
    printf("foo !!\n");

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

    foo(2);

    printf("main exit !!\n");

    __sanitizer_exit();
    return 1;
}



