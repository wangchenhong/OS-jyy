#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



#define DEFAULT_NUMLINES 10
#define BLOCK_SIZE 4096

void print_last_lines(int fd, int num_lines);

int main(int argc, char *argv[]){
    int num_lines = DEFAULT_NUMLINES;
    char *filename = NULL;

    if(argc < 2){
        fprintf(stderr, "Usage: %s [-n num_lines] filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(argc > 2 && strcmp(argv[1], "-n") == 0){
        if(argc < 4){
            fprintf(stderr, "Usage: %s [-n num_lines] filename\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        num_lines = atoi(argv[2]);
        filename = argv[3];
    }else{
        filename = argv[1];
    }

    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    print_last_lines(fd, num_lines);

    if(close(fd) == -1){
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void print_last_lines(int fd, int num_lines){
    struct stat file_stat;
    if(fstat(fd, &file_stat) == -1){
        perror("Error getting file size");
        exit(EXIT_FAILURE);
    }

    off_t file_size = file_stat.st_size;
    if(file_size == 0){
        return ;
    }

    char buffer[BLOCK_SIZE];
    ssize_t bytes_read;
    int line_count = 0;
    off_t current_pos = file_size;

    while(current_pos > 0 && line_count <= num_lines){
        off_t seek_pos = (current_pos >= BLOCK_SIZE) ? current_pos - BLOCK_SIZE : 0;
        if(lseek(fd, seek_pos, SEEK_SET) == -1){
            perror("Error seeking in file");
            return ;
        }
        bytes_read = read(fd, buffer, current_pos - seek_pos);
        if(bytes_read < 0){
            perror("Error reading file");
            return ;
        }

        for(ssize_t i = bytes_read - 1; i >= 0 ; i--){
            if(buffer[i] == '\n'){
                line_count++;
            }
        }
        current_pos = seek_pos;
    }
    if(lseek(fd, current_pos, SEEK_SET) == -1){
        perror("Error seeking in file");
        return ;
    }
    bytes_read = read(fd, buffer, file_size - current_pos);
    if(bytes_read < 0){
        perror("Error reading file");
        return ;
    }
    ssize_t start = 0;
    ssize_t end = bytes_read;
    for(ssize_t i = bytes_read - 1; i >= 0 && line_count < num_lines; i--){
        if(buffer[i] == '\n'){
            if(line_count == num_lines - 1){
                start = i + 1;
            }
            line_count++;
        }
    }
    if(line_count < num_lines && start > 0){
        start = 0; // If we didn't find enough lines, print from the start
    }
    if(write(STDOUT_FILENO, buffer + start, end - start) < 0){
        perror("Error writing to stdout");
        return ;
    }
    if(write(STDOUT_FILENO, "\n", 1) < 0){
        perror("Error writing newline to stdout");
        return ;
    }
    // Ensure the output is flushed
    if(fflush(stdout) == EOF){
        perror("Error flushing stdout");
        return ;
    }
    // If we reach here, everything was successful
    return ;

}