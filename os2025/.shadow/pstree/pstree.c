#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#define HASH_TABLE_SIZE 1024

//进程节点
typedef struct ProcessNode{
    pid_t pid;
    pid_t ppid;
    char name[256];
    struct ProcessNode *parent; 
    struct ProcessNode *children;  //指向第一个子节点
    struct ProcessNode *sibling;   //指向下一个兄弟节点
}ProcessNode;

//哈希表节点
typedef struct HashNode{
    pid_t pid;                      //键
    ProcessNode *process;        //值
    struct HashNode * next;        //下一个节点（用于解决冲突）
} HashNode;

//哈希表结构
typedef struct {
    HashNode **buckets;          //桶数组
    size_t size;                    //哈希表大小
}HashTable;

int is_numeric(const char *str){
    while(*str){
        if(!isdigit(*str)){
            return 0;
        }
        str++;
    }
    return 1;
}

HashTable* create_hash_table(size_t size){
    HashTable *ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->buckets = calloc(size, sizeof(HashNode*));
    return ht;
}

void hash_table_insert(HashTable *ht, pid_t pid, ProcessNode *process){
    size_t index = pid % ht->size;  //简单哈希函数

    HashNode *new_node = malloc(sizeof(HashNode));
    new_node->pid = pid;
    new_node->process = process;

    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
}

//从哈希表查找进程节点
ProcessNode* hash_table_lookup(HashTable *ht, pid_t pid){
    size_t index = pid % ht->size;
    HashNode *node = ht->buckets[index];

    while(node != NULL){
        if(node->pid == pid){
            return node->process;
        }
        node = node->next;
    }
    return NULL;
}



int main() {
    DIR *proc_dir;
    struct dirent *entry;

    HashTable *ht = create_hash_table(HASH_TABLE_SIZE);

    proc_dir = opendir("/proc");
    if(proc_dir == NULL){
        perror("fail to open /proc");
        return EXIT_FAILURE;
    }

    while((entry = readdir(proc_dir)) != NULL){
        if(entry->d_type == DT_DIR && is_numeric(entry->d_name)){
            pid_t pid = atoi(entry->d_name);
            
            pid_t ppid = read_ppid(pid);
            char *name = read_process_name(pid);

            

            printf("%s", entry->d_name);
        }
    }
    closedir(proc_dir);
}
