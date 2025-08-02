#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    // printf("location fo code : %p\n", main);
    // printf("location fo heap : %p\n", malloc(100e6));
    // int x = 3;
    // printf("location of stack : %p\n", &x);
    char *src = "hello";
    char *dst = (char *)malloc(strlen(src));
    strcpy(dst, src);
    return 0;
}