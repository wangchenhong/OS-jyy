#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fnmatch.h>

struct SearchOptions{
    const char *name_pattern;
    char type;
};

void find_recursively(const char *base_path, struct SearchOptions *opts){
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
            snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);

            struct stat st;
            if(stat(path, &st) == -1){
                printf("Error getting stats for %s: \n", path);
                perror("stat");
                continue ;
            }
            int type_match = 0;
            if(opts->type == 0){
                type_match = 1;
            }else if(opts->type == 'd' && S_ISDIR(st.st_mode)){
                type_match = 1;
            }else if(opts->type == 'f' && S_ISREG(st.st_mode)){
                type_match = 1;
            }

            int name_match = 0;
            if(opts->name_pattern == NULL){
                name_match = 1;
            }else if(fnmatch(opts->name_pattern, dp->d_name, 0) == 0){
                name_match = 1;
            }

            if(type_match && name_match){
                printf("%s\n", path);
            }

            if(dp->d_type == DT_DIR){
                find_recursively(path, opts);
            }
        }
    }
    closedir(dir);
}

void print_usage(const char *prog_name){
    fprintf(stderr, "Usage: %s [directory] [options]\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -n <name_pattern>  Search for files matching the name pattern\n");
    fprintf(stderr, "  -t <type>          Search for files of type 'd' (directory) or 'f' (file)\n");
    fprintf(stderr, "  -h                 Show this help message\n");
}

int main(int argc, char *argv[]){
    if(argc < 2){
        print_usage(argv[0]);
        return 0;
    }

    const char *start_path = argv[1];
    struct SearchOptions opts = {.name_pattern = NULL, .type = 0};

    for(int i = 2; i < argc; i++){
        if(strcmp(argv[i], "-n") == 0 && i + 1 < argc){
            opts.name_pattern = argv[++i];
        }else if(strcmp(argv[i], "-t") == 0 && i + 1 < argc){
            opts.type = argv[++i][0];
            if(opts.type != 'd' && opts.type != 'f'){
                fprintf(stderr, "Error: Invalid type '%c'. Use 'd' for directory or 'f' for file.\n", opts.type);
                exit(EXIT_FAILURE);
            }
        }
    }
    struct stat st;
    if(stat(start_path, &st) == -1 || !S_ISDIR(st.st_mode)){
        fprintf(stderr, "Error: %s is not a directory or does not exist.\n", start_path);
        exit(EXIT_FAILURE);
    }
    find_recursively(start_path, &opts);
    return 0;
}