#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void print_file_info(char *file_path){
    struct stat file_stat;
    if(stat(file_path, &file_stat) == -1){
        perror("stat() error");
        return ;
    }

    printf("File: %s\n", file_path);

    //文件权限
    printf("Permissions: ");
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %lu", file_stat.st_nlink);

    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);

    if(pw != NULL){
        printf(" %s", pw->pw_name);
    }else{
        printf(" %d", file_stat.st_uid);
    }

    if(gr != NULL){
        printf(" %s", gr->gr_name);
    }else{
        printf(" %d", file_stat.st_gid);
    }
    printf(" %lld", (long long)file_stat.st_size);
    printf("\n");
}


int main(int argc, char *argv[]){
    int long_format = 0;
    char *dir_path = ".";
    int opt;

    while((opt = getopt(argc, argv, "l")) != -1){
        switch(opt){
            case 'l':
                long_format = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(optind < argc){
        dir_path = argv[optind];
    }else{
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            dir_path = cwd;
        }else{
            perror("getcwd() error");
            return 1;
        }
    }

    DIR *dir;
    struct dirent *entry;

    if((dir = opendir(dir_path)) == NULL){
        perror("opendir() error");
        return 1;
    }

    while((entry = readdir(dir)) != NULL){
        if(long_format){
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            print_file_info(full_path);
        }else{
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);

    return 0;

}