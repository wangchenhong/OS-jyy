#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
// #include <testkit.h>
#include "labyrinth.h"
#include <getopt.h>

#define VERSION "1.0"

const char *map_file = NULL;
char player_id = '\0';
const char *direction = NULL;
bool version_flag = false;
bool other_options = false;


int main(int argc, char *argv[]) {
    // TODO: Implement this function
    int opt;
    int option_index = 0;
    Labyrinth labyrinth;

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
                if(optarg == NULL){
                    fprintf(stderr, "Option '-m/--map' requires an argument.\n");
                    printUsage();
                    return 1;
                }
                other_options = true;
                map_file = optarg;
                break;
            }
            case 'p':{
                // 处理玩家ID
                if(optarg == NULL){
                    fprintf(stderr, "Option '-p/--player' requires an argument.\n");
                    printUsage();
                    return 1;
                }
                other_options = true;
                player_id = optarg[0];
                break;
            }
            case '0':{
                if(strcmp(long_options[option_index].name, "move") == 0){
                    // 处理移动方向
                    if(optarg == NULL){
                        fprintf(stderr, "Option '--move' requires an argument.\n");
                        printUsage();
                        return 1;
                    }
                    other_options = true;
                    direction = optarg;
                }else if(strcmp(long_options[option_index].name, "version") == 0){
                    // 打印版本信息
                    version_flag = true;
                }
                break;
            }
        }
    }
    if(version_flag && other_options){
        fprintf(stderr, "Error: --version and other options cannot be included simultaneously.\n");
        return 1;
    }
    if(version_flag){
        printUsage();
    }
    //加载地图
    if(loadMap(&labyrinth, map_file)){
        printf("Map loaded successfully from %s\n", map_file);
        // 打印地图
        printMap(&labyrinth);
    }else{
        fprintf(stderr, "Failed to load map from %s\n", map_file);
        return 1;
    }
    // 检查玩家ID是否有效
    if(isValidPlayer(player_id)){
        printf("Player ID %c is vaild\n", player_id);
    }else{
        fprintf(stderr, "Invalid player ID: %c\n", player_id);
        return 1;
    }
    //移动玩家
    if(!movePlayer(&labyrinth, player_id, direction)){
        return 1;
    }




    return 0;
}

void printUsage() {
    printf("Labyrinth Game Version: %s\n", VERSION);
    printf("Usage:\n");
    printf("  labyrinth --map map.txt --player id\n");
    printf("  labyrinth -m map.txt -p id\n");
    printf("  labyrinth --map map.txt --player id --move direction\n");
    printf("  labyrinth --version\n");
}

void printMap(Labyrinth *labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        printf("%s\n", labyrinth->map[i]);
    }
}

bool isValidPlayer(char playerId) {
    // TODO: Implement this function
    if(playerId >= '0' && playerId <= '9') {
        return true;
    }
    return false;
}

bool loadMap(Labyrinth *labyrinth, const char *filename) {
    // TODO: Implement this function
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return false;
    }
    char line[MAX_COLS + 2]; // +2 for newline and null terminator
    int row = 0;
    while(fgets(line, sizeof(line), file)){
        size_t len = strlen(line);
        if(len > 0 && line[len - 1] == '\n'){
            line[len - 1] = '\0';
        }
        if(len > MAX_COLS){
            fprintf(stderr, "Exceeds maximum column limit of %d\n", MAX_COLS);
            fclose(file);
            return false;
        }
        if(row >= MAX_ROWS){
            fprintf(stderr, "Exceeds maximum row limit of %d\n", MAX_ROWS);
            fclose(file);
            return false;
        }
        strncpy(labyrinth->map[row], line, MAX_COLS);
        labyrinth->map[row][len] = '\0';
        row++;
    }
    labyrinth->rows = row;
    labyrinth->cols = strlen(labyrinth->map[0]);
    if(!isConnected(labyrinth)){
        fclose(file);
        return false;
    }
    return true;
}

Position findPlayer(Labyrinth *labyrinth, char playerId) {
    // TODO: Implement this function
    Position pos = {-1, -1};
    for(int row = 0; row < labyrinth->rows; row++){
        for(int col = 0; col < labyrinth->cols; col++){
            if(labyrinth->map[row][col] == playerId){
                pos.row = row;
                pos.col = col;
                return pos;
            }
        }
    }
    return pos;
}

Position findFirstEmptySpace(Labyrinth *labyrinth) {
    // TODO: Implement this function
    Position pos = {-1, -1};
    for(int row = 0; row <labyrinth->rows; row++){
        for(int col = 0; col < labyrinth->cols; col++){
            if(isEmptySpace(labyrinth, row, col)){
                pos.row = row;
                pos.col = col;
                return pos;
            }
        }
    }
    return pos;
}

bool isEmptySpace(Labyrinth *labyrinth, int row, int col) {
    // TODO: Implement this function
    if(row < 0 || row >= labyrinth->rows || col < 0 || col >= labyrinth->cols){
        return false;
    }
    if(labyrinth->map[row][col] == '.' || (labyrinth->map[row][col] >= '0' && labyrinth->map[row][col] <= '9')){
        return true;
    }
    return false;
}

bool movePlayer(Labyrinth *labyrinth, char playerId, const char *direction) {
    // TODO: Implement this function
    Position playerPos = findPlayer(labyrinth, playerId);
    if(playerPos.row == -1 && playerPos.col == -1){
        playerPos = findFirstEmptySpace(labyrinth);
    }
    if(playerPos.row == -1 && playerPos.col == -1){
        fprintf(stderr, "No empty space found for player %c\n", playerId);
        return false;
    }
    // 初始化偏移量
    int rowOffset = 0;
    int colOffset = 0;
    if(strcmp(direction, "up") == 0){
        rowOffset = -1;
    }else if(strcmp(direction, "down") == 0){
        rowOffset = 1;
    }else if(strcmp(direction, "left") == 0){
        colOffset = -1;
    }else if(strcmp(direction, "right") == 0){
        colOffset = 1;
    }else{
        fprintf(stderr, "Invalid direction: %s\n", direction);
        return false;
    }
    // 计算新位置
    int newRow = playerPos.row + rowOffset;
    int newCol = playerPos.col + colOffset;
    // 检查新位置是否有效
    if(newRow < 0 || newRow >= labyrinth->rows || newCol < 0 || newCol >= labyrinth->cols){
        fprintf(stderr, "Move out of bounds: (%d, %d)\n", newRow, newCol);
        return false;
    }
    if(!isEmptySpace(labyrinth, newRow, newCol) || (labyrinth->map[newRow][newCol] >= '0' && labyrinth->map[newRow][newCol] <= '9')){
        fprintf(stderr, "Cannot move to ouccupied space: (%d, %d)\n", newRow, newCol);
        return false;
    }
    // 移动玩家
    labyrinth->map[playerPos.row][playerPos.col] = '.';
    labyrinth->map[newRow][newCol] = playerId;
    printf("Player %c moved to (%d, %d)\n", playerId, newRow, newCol);
    saveMap(labyrinth, "maps/map.txt");
    printMap(labyrinth);
    return true; 
}

bool saveMap(Labyrinth *labyrinth, const char *filename) {
    // TODO: Implement this function
    FILE *file = fopen(filename, "w");
    if(!file){
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        return false;
    }
    for(int row = 0; row < labyrinth->rows; row++){
        if(fputs(labyrinth->map[row], file) == EOF){
            fprintf(stderr, "Error writing to file: %s\n", filename);
            fclose(file);
            return false;
        }
        fputc('\n', file);
    }
    fclose(file);
    printf("Map saved successfully to %s\n", filename);
    return true;
}

// Check if all empty spaces are connected using DFS
void dfs(Labyrinth *labyrinth, int row, int col, bool visited[MAX_ROWS][MAX_COLS]) {
    // TODO: Implement this function
    //定义偏移量数组
    int rowOffsets[] = {-1, 1, 0, 0};
    int colOffsets[] = {0, 0, -1, 1};
    for(int i = 0; i < 4; i++){
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];
        if(newRow >= 0 && newRow < labyrinth->rows &&
           newCol >= 0 && newCol < labyrinth->cols &&
           !visited[newRow][newCol] && isEmptySpace(labyrinth, newRow, newCol)){
            visited[newRow][newCol] = true;
            dfs(labyrinth, newRow, newCol, visited);
        }
    }
}

bool isConnected(Labyrinth *labyrinth) {
    // TODO: Implement this function
    bool visited[MAX_ROWS][MAX_COLS] = {false};
    Position start = findFirstEmptySpace(labyrinth);
    if(start.row == -1 && start.col == -1){
        fprintf(stderr, "No empty space found in the labyrinth\n");
        return false;
    }
    visited[start.row][start.col] = true;
    dfs(labyrinth, start.row, start.col, visited);
    // 检查所有空白空间是否都被访问过
    for(int row = 0; row < labyrinth->rows; row++){
        for(int col = 0; col < labyrinth->cols; col++){
            if(isEmptySpace(labyrinth, row, col) && !visited[row][col]){
                fprintf(stderr, "Empty space at (%d, %d) is not connected\n", row, col);
                return false;
            }
        }
    }
    // printf("All empty spaces are connected\n");
    return true;
}
