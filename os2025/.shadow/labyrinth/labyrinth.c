#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <testkit.h>
#include "labyrinth.h"

#define VERSION "1.0"

// 定义方向枚举
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

static char *map_file = NULL;
static char *player_id = NULL;
static enum Direction move_dir = NONE;

//将字符串转换为方向枚举
enum Direction parse_direction(const char *str){
    if(strcmp(str, "up") == 0) return UP;
    if(strcmp(str, "down") == 0) return DOWN;
    if(strcmp(str, "left") == 0) return LEFT;
    if(strcmp(str, "right") == 0) return RIGHT;
    return NONE;
}

int main(int argc, char *argv[]) {
    // TODO: Implement this function
    int opt;
    int option_index = 0;

    //长选项定义
    static struct option long_options[] = {
        {"map", required_argument, 0, 'm'},
        {"player", required_argument, 0, 'p'},
        {"move", required_argument, 0, '0'},
        {"version", no_argument, 0, '0'},
        {0, 0, 0, 0}
    };

    //解析命令行参数
    while((opt = getopt_long(argc, argv, "m:p:0:", long_options, &option_index)) != -1){
        switch(opt){
            case 'm':{
                // 处理地图文件
                map_file = optarg;
                if(loadMap(&Labyrinth, map_file)){
                    printf("Map loaded successfully from %s\n", map_file);
                }else{
                    fprintf(stderr, "Failed to load map from %s\n", map_file);
                    return 1;
                }
                break;
            }
            case 'p':{
                // 处理玩家ID
                player_id = optarg;
                if(isValidPlayer(player_id)){
                    printf("Player ID %s is vaild\n", player_id);
                }else{
                    fprintf(stderr, "Invalid player ID: %s\n", player_id);
                    return 1;
                }
            }
            case '0':{
                if(strcmp(long_options[option_index].name, "move") == 0){
                    // 处理移动方向
                    move_dir = parse_direction(optarg);
                    if(move_dir == NONE){
                        fprintf(stderr, "Invalid move direcrtion: %s\n", optarg);
                        return 1;
                    }
                }else if(strcmp(long_options[option_index].name, "version") == 0){
                    // 打印版本信息
                    printf("Labyrinth Game Version: %s\n", VERSION);
                }
            }
        }
    }

    return 0;
}

void printUsage() {
    printf("Usage:\n");
    printf("  labyrinth --map map.txt --player id\n");
    printf("  labyrinth -m map.txt -p id\n");
    printf("  labyrinth --map map.txt --player id --move direction\n");
    printf("  labyrinth --version\n");
}

bool isValidPlayer(char playerId) {
    // TODO: Implement this function
    return false;
}

bool loadMap(Labyrinth *labyrinth, const char *filename) {
    // TODO: Implement this function
    return false;
}

Position findPlayer(Labyrinth *labyrinth, char playerId) {
    // TODO: Implement this function
    Position pos = {-1, -1};
    return pos;
}

Position findFirstEmptySpace(Labyrinth *labyrinth) {
    // TODO: Implement this function
    Position pos = {-1, -1};
    return pos;
}

bool isEmptySpace(Labyrinth *labyrinth, int row, int col) {
    // TODO: Implement this function
    return false;
}

bool movePlayer(Labyrinth *labyrinth, char playerId, const char *direction) {
    // TODO: Implement this function
    return false;
}

bool saveMap(Labyrinth *labyrinth, const char *filename) {
    // TODO: Implement this function
    return false;
}

// Check if all empty spaces are connected using DFS
void dfs(Labyrinth *labyrinth, int row, int col, bool visited[MAX_ROWS][MAX_COLS]) {
    // TODO: Implement this function
}

bool isConnected(Labyrinth *labyrinth) {
    // TODO: Implement this function
    return false;
}
