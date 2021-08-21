

#include <stdio.h>



int foo1(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else {
        foo2(b);
    }

    return 0;
}

int foo2(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else if (b==3) {
        ;
    } else {
        foo3(b);
    }

    return 0;
}

int foo3(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else if (b==3) {
        ;
    } else if (b==4) {
        ;
    } else {
        foo4(b);
    }

    return 0;
}

int foo4(int b) {
    printf("foo1 !!\n");

    if (b==2) {
        foo5(1);
    } else if (b==4) {
        ;
    } else if (b==6) {
        ;
    } else if (b==9) {
        ;
    } else if (b==2) {
        ;
    }

    return 0;
}

int foo5(int b) {
    return 1;
}


int main(int argc,char** argv) {
    __sanitizer_enter();

    printf("main running !!!\n");

    foo1(2);

    printf("main exit !!\n");

    __sanitizer_exit();
    return 1;
}



