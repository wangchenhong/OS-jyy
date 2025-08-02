#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Only one argument is expected.\n");
        return 1;
    }
    int MB = atoi(argv[1]);
    if(MB <= 0){
        fprintf(stderr, "Argument must be a positive integer.\n");
        return 1;
    }

    int size = MB * 1024 * 1024 * 256;

    int *arr = malloc(size * sizeof(int));
    if(arr == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    int x[10];
    printf("%ld\n", sizeof(x));

    // while(1){
    //     for(int i = 0; i < size; ++i){
    //         arr[i] = i;
    //     }   
    // }

    free(arr);
    
    return 0;
}