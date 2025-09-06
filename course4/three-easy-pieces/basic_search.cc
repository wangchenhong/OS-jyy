#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


void list_directory_recursively(const char *base_path){
    char path[1024];
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if(!dir){
        perror("opendir");
        return;
    }

    printf("Listing directory: %s\n", base_path);

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            snprintf(path, sizeof(path), "%s%s", base_path, dp->d_name);
            printf("%s\n", path);

            if(dp->d_type == DT_DIR){
                list_directory_recursively(path);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    const char *start_path = (argc > 1) ? argv[1] : ".";

    struct stat st;
    if(stat(start_path, &st) == -1 || !S_ISDIR(st.st_mode)){
        fprintf(stderr, "Error: %s is not a directory or does not exist.\n", start_path);
        exit(EXIT_FAILURE);
    }
    list_directory_recursively(start_path);
    return 0;
}