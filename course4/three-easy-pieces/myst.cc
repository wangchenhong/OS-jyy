#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>

int main(int argc, char *argv[]){
    char *file_path = argv[1];
    struct stat st;
    if (stat(file_path, &st) == -1){
        perror("stat");
        exit(EXIT_FAILURE);
    }
    //Print out file size, number of blocks allocated, reference(link) count, and so forth
    printf("File size: %lld bytes\n", (long long)st.st_size);
    printf("Blocks allocated: %lld\n", (long long)st.st_blocks);
    printf("Reference count: %ld\n", (long)st.st_nlink);
    printf("File type: ");
    switch (st.st_mode & S_IFMT){
        case S_IFREG: printf("Regular file\n"); break;
        case S_IFDIR: printf("Directory\n"); break;
        case S_IFCHR: printf("Character device\n"); break;
        case S_IFBLK: printf("Block device\n"); break;
        case S_IFIFO: printf("FIFO/pipe\n"); break;
        case S_IFLNK: printf("Symblic link\n"); break;
        case S_IFSOCK: printf("Socket\n"); break;
        default: printf("Unknown file type\n");
    }

}