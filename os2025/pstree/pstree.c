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
    if(ht == NULL){
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    ht->size = size;
    ht->buckets = calloc(size, sizeof(HashNode*));
    return ht;
}

void hash_table_insert(HashTable *ht, pid_t pid, ProcessNode *process){
    size_t index = pid % ht->size;  //简单哈希函数

    HashNode *new_node = malloc(sizeof(HashNode));
    if(new_node == NULL){
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
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

int read_process_info(pid_t pid, pid_t *ppid, char *name, size_t name_size){
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *fp = fopen(path, "r");
    if(!fp){
        return -1;
    }

    char buffer[1024];
    if(fgets(buffer, sizeof(buffer), fp) == NULL){
        fclose(fp);
        return -1;
    }
    fclose(fp);

    //查找进程名
    char *start = strchr(buffer, '(');
    char *end = strchr(buffer, ')');
    
    if(!start || !end || start >= end){
        return -1;
    }

    *start++;
    size_t name_len = end - start;
    if(name_len > name_size - 1){
        name_len = name_size -1;    //防止缓冲区溢出
    }

    strncpy(name, start, name_len);
    name[name_len] = '\0';

    //跳过空格和进程状态字段
    char *p = end + 1;
    while(*p == ' ') p++;
    if(*p == '\0') return -1;
    p++;
    while(*p == ' ') p++;
    if(!p) return -1;

    *ppid = atoi(p);

    return 0;

}

//打印单个进程节点及其子节点
void print_node(ProcessNode *node, int depth, int is_last){
    for(int i = 0; i < depth; i++){
        if(i == depth - 1){
            printf(is_last ? "'--" : "|--");
        }else{
            printf("    ");
        }
    }
    printf("%s(%d)\n", node->name, node->pid);

    ProcessNode *child = node->children;
    while(child != NULL){
        int child_is_last = (child->sibling == NULL);
        print_node(child, depth + 1, child_is_last);
        child = child->sibling;
    }
}

void print_process_tree(ProcessNode *root){
    if(root == NULL){
        printf("没有找到进程树\n");
        return ;
    }
    printf("%s(%d)\n", root->name, root->pid);

    //递归打印子节点
    ProcessNode *child = root->children;
    while(child != NULL){
        int child_is_last = (child->sibling == NULL);
        print_node(child, 1, child_is_last);
        child = child->sibling;
    }
}

void free_process_node(ProcessNode *node){
    if(node == NULL){
        return ;
    }
    ProcessNode *child = node->children;
    while(child != NULL){
        ProcessNode *next = child->sibling;
        free_process_node(child);
        child = next;
    }
    free(node);
}

void free_resources(HashTable *ht){
    if(ht == NULL){
        return;
    }

    for(size_t i = 0; i < ht->size; ++i){
        HashNode *hash_node = ht->buckets[i];
        while(hash_node != NULL){
            // free_process_node(hash_node->process);
            // hash_node->process = NULL;  //避免悬空指针

            HashNode *next = hash_node->next;
            free(hash_node->process);
            free(hash_node);
            hash_node= next;
        }
        ht->buckets[i] = NULL;
    }
    free(ht->buckets);
    free(ht);
}

int compare_pid(const void *a, const void *b){
    const ProcessNode *nodeA = *(const ProcessNode **)a;
    const ProcessNode *nodeB = *(const ProcessNode **)b;
    return nodeA->pid - nodeB->pid;
}

ProcessNode** sort_children(ProcessNode *parent, int *count){
    if(parent == NULL || parent->children == NULL){
        *count = 0;
        return NULL;
    }
    int child_count = 0;
    ProcessNode *child = parent->children;
    while(child != NULL){
        child_count++;
        child = child->sibling;
    }

    ProcessNode **child_array = malloc(child_count * sizeof(ProcessNode *));
    if(child_array == NULL){
        *count = 0;
        return NULL;
    }

    child = parent->children;
    for(int i = 0; i < child_count; i++){
        child_array[i] = child;
        child = child->sibling;
    }

    qsort(child_array, child_count, sizeof(ProcessNode *), compare_pid);

    *count = child_count;
    return child_array;
}

//打印单个进程节点及其子节点
void print_node_sorted(ProcessNode *node, int depth, int is_last){
    for(int i = 0; i < depth; i++){
        if(i == depth - 1){
            printf(is_last ? "'--" : "|--");
        }else{
            printf("    ");
        }
    }
    printf("%s(%d)\n", node->name, node->pid);

    int child_count = 0;
    ProcessNode **sorted_child = sort_children(node, &child_count);

    for(int i = 0; i < child_count; i++){
        int child_is_last = ( i == child_count - 1);
        print_node_sorted(sorted_child[i], depth + 1, child_is_last);
    }
    
    if(sorted_child != NULL){
        free(sorted_child);
    }
}

void print_process_tree_sorted(ProcessNode *root){
    if(root == NULL){
        printf("没有找到进程树\n");
        return ;
    }
    printf("%s(%d)\n", root->name, root->pid);

    int child_count = 0;
    ProcessNode **sorted_child = sort_children(root, &child_count);

    for(int i = 0; i < child_count; i++){
        int child_is_last = (i == child_count - 1);
        print_node_sorted(sorted_child[i], 1, child_is_last);
    }

    if(sorted_child != NULL){
        free(sorted_child);
    }
}


int main(int argc, char *argv[]) {
    int sorted = 0;
    if(argc > 1 && strcmp(argv[1], "-n") == 0){
        sorted = 1;
    }
    
    
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
            
            pid_t ppid;
            char name[256];

            if(read_process_info(pid, &ppid, name, sizeof(name)) != 0){
                continue;
            }

            ProcessNode *node = malloc(sizeof(ProcessNode));
            if(node == NULL){
                perror("内存分配失败");
                exit(EXIT_FAILURE);
            }
            node->pid = pid;
            node->ppid = ppid;
            strncpy(node->name, name, sizeof(node->name) - 1);
            node->name[sizeof(node->name) - 1] = '\0';
            node->parent = NULL;
            node->children = NULL;
            node->sibling = NULL;

            hash_table_insert(ht, pid, node);
        }
    }
    closedir(proc_dir);

    //建立父子关系
    //遍历哈下表的所有桶
    for(size_t i = 0; i < ht->size; i++){
        HashNode *hashnode = ht->buckets[i];
        while(hashnode != NULL){
            ProcessNode *node = hashnode->process;

            if(node->ppid != 0){
                ProcessNode *parent = hash_table_lookup(ht, node->ppid);

                if(parent != NULL){
                    node->parent = parent;
                    node->sibling = parent->children;
                    parent->children = node;
                }else{
                    fprintf(stderr, "找不到父进程%d(子进程%d)\n", node->ppid, node->pid);
                }
            }
            hashnode = hashnode->next;
        }
    }

    ProcessNode *root = hash_table_lookup(ht, 1);
    if(root != NULL){
        if(sorted){
            print_process_tree_sorted(root);
        }else{
            print_process_tree(root);
        }
    }   
    free_resources(ht);

    return 0;
}
