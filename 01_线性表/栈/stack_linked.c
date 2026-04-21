/*
 * 链栈（Linked Stack）实现
 * 考研408数据结构 - 栈
 *
 * 用链表实现栈，链表头部作为栈顶（便于O(1)插入删除）
 * 不需要头结点
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 链栈结点定义 */
typedef struct StackNode {
    int data;
    struct StackNode *next;
} StackNode;

/* 链栈类型（top指针即为链表头指针） */
typedef struct {
    StackNode *top;     /* 栈顶指针 */
    int size;           /* 栈中元素个数 */
} LinkedStack;

/* 初始化链栈 */
void InitStack(LinkedStack *S) {
    S->top = NULL;
    S->size = 0;
}

/* 判断链栈是否为空 */
bool StackEmpty(LinkedStack *S) {
    return S->top == NULL;
}

/*
 * 入栈：在链表头部插入新结点（头插法）
 * 时间复杂度：O(1)
 */
bool Push(LinkedStack *S, int x) {
    StackNode *p = (StackNode *)malloc(sizeof(StackNode));
    if (!p) { printf("内存分配失败\n"); return false; }
    p->data = x;
    p->next = S->top;   /* 新结点指向原栈顶 */
    S->top = p;         /* 更新栈顶指针 */
    S->size++;
    return true;
}

/*
 * 出栈：删除链表头部结点
 * 时间复杂度：O(1)
 */
bool Pop(LinkedStack *S, int *x) {
    if (StackEmpty(S)) { printf("栈空，无法出栈\n"); return false; }
    StackNode *p = S->top;
    *x = p->data;
    S->top = p->next;   /* 栈顶指针后移 */
    free(p);            /* 释放结点内存 */
    S->size--;
    return true;
}

/* 读取栈顶元素（不出栈） */
bool GetTop(LinkedStack *S, int *x) {
    if (StackEmpty(S)) { printf("栈空\n"); return false; }
    *x = S->top->data;
    return true;
}

/* 打印链栈（从栈顶到栈底） */
void PrintStack(LinkedStack *S) {
    printf("链栈（顶->底）: TOP");
    StackNode *p = S->top;
    while (p != NULL) {
        printf(" -> %d", p->data);
        p = p->next;
    }
    printf(" -> NULL\n");
}

/* 销毁链栈，释放所有内存 */
void DestroyStack(LinkedStack *S) {
    int x;
    while (!StackEmpty(S)) Pop(S, &x);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 链栈基本操作演示 =====\n\n");

    LinkedStack S;
    InitStack(&S);

    printf("--- 入栈操作 ---\n");
    for (int i = 1; i <= 5; i++) {
        Push(&S, i * 10);
        printf("入栈 %d -> ", i * 10);
        PrintStack(&S);
    }

    int top;
    GetTop(&S, &top);
    printf("\n栈顶元素 = %d，栈大小 = %d\n", top, S.size);

    printf("\n--- 出栈操作 ---\n");
    int x;
    while (!StackEmpty(&S)) {
        Pop(&S, &x);
        printf("出栈 %d -> ", x);
        PrintStack(&S);
    }

    DestroyStack(&S);
    return 0;
}
