// int f(int n) { return (n<=1) ? 1 : f(n-1) + g(n-2); }
// int g(int n) { return (n<=1) ? 1 : f(n+1) + g(n-1); }


#include <stdio.h>
#include <stdlib.h>

int f(int n);
int g(int n);

int f(int n) {
    if (n <= 1) return 1;
    return f(n - 1) + g(n - 2);
}
int g(int n) {
    if (n <= 1) return 1;
    return f(n + 1) + g(n - 1);
}

typedef struct{
    int n;      // 当前参数n
    int flag;     // 0表示f函数，1表示g函数
    int stage;  // 执行阶段：0表示还未进行左递归调用，1表示左递归调用结束，2表示右递归调用结束，整个帧结束
}Frame;

#define STACK_SIZE 1000
Frame stack[STACK_SIZE];
int top = -1;

void push(Frame frame) {
    if (top < STACK_SIZE - 1) {
        stack[++top] = frame;
    } else {
        fprintf(stderr, "Stack overflow!\n");
        exit(1);
    }
}

Frame pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
}

int isEmpty() {
    return top == -1;
}

int f_g(int n, int flag){
    Frame init = {n, flag, 0};
    push(init);
    int ans = 0;

    while(!isEmpty()){
        Frame *current = &stack[top];

        if(current->n <= 1 && current->stage == 0){
            ans += 1;
            pop();
        }else if(current->stage == 0){
            current->stage = 1; // 标记已进入左递归阶段
            if(current->flag == 0){
                Frame leftFrame = {current->n - 1, 0, 0}; // f(n-1)
                push(leftFrame);
            }else if(current->flag == 1){
                Frame leftFrame = {current->n + 1, 0, 0}; // g(n+1)
                push(leftFrame);
            }
        }else if(current->stage == 1){
            current->stage = 2; // 标记已进入右递归阶段
            if(current->flag == 0){
                Frame rightFrame = {current->n - 2, 1, 0}; // g(n-2)
                push(rightFrame);
            }else if(current->flag == 1){
                Frame rightFrame = {current->n - 1, 1, 0}; // g(n-1)
                push(rightFrame);
            }
        }else if(current->stage == 2){
            pop();
        }
    }
    return ans;
}

int main() {
    int n = 10; // Example input
    int result_f = f(n);
    int result_g = g(n);
    int result_ff = f_g(n, 0);
    int result_gg = f_g(n, 1);
    printf("f(%d) = %d\n", n, result_f);
    printf("ff(%d) = %d\n", n, result_ff);
    printf("g(%d) = %d\n", n, result_g);
    printf("gg(%d) = %d\n", n, result_gg);
    return 0;
}