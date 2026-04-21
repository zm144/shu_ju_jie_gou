/*
 * 链队列（Linked Queue）实现
 * 考研408数据结构 - 队列
 *
 * 用带头结点的链表实现队列
 * front指向头结点，rear指向尾结点（最后一个数据结点）
 * 优点：无容量限制
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 链队列结点定义 */
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode;

/* 链队列定义（含头尾指针） */
typedef struct {
    QNode *front;   /* 队头指针，指向头结点 */
    QNode *rear;    /* 队尾指针，指向最后一个数据结点 */
} LinkQueue;

/* 初始化链队列（创建头结点） */
void InitQueue(LinkQueue *Q) {
    Q->front = Q->rear = (QNode *)malloc(sizeof(QNode));
    if (!Q->front) { printf("内存分配失败\n"); exit(1); }
    Q->front->next = NULL;
}

/* 判断队列是否为空（front == rear 即头结点后无数据结点） */
bool QueueEmpty(LinkQueue *Q) {
    return Q->front == Q->rear;
}

/*
 * 入队（EnQueue）：在队尾插入新结点
 * 时间复杂度：O(1)（通过rear指针直接定位尾部）
 */
void EnQueue(LinkQueue *Q, int x) {
    QNode *p = (QNode *)malloc(sizeof(QNode));
    if (!p) { printf("内存分配失败\n"); return; }
    p->data = x;
    p->next = NULL;
    Q->rear->next = p;  /* 新结点链接到队尾 */
    Q->rear = p;        /* rear指针后移 */
}

/*
 * 出队（DeQueue）：删除队头结点（头结点后的第一个数据结点）
 * 时间复杂度：O(1)
 */
bool DeQueue(LinkQueue *Q, int *x) {
    if (QueueEmpty(Q)) { printf("队空，无法出队\n"); return false; }
    QNode *p = Q->front->next;  /* p为第一个数据结点 */
    *x = p->data;
    Q->front->next = p->next;   /* 头结点绕过p */
    if (Q->rear == p)           /* 删除的是最后一个结点，更新rear */
        Q->rear = Q->front;
    free(p);
    return true;
}

/* 读取队头元素（不出队） */
bool GetHead(LinkQueue *Q, int *x) {
    if (QueueEmpty(Q)) { printf("队空\n"); return false; }
    *x = Q->front->next->data;
    return true;
}

/* 打印队列所有元素 */
void PrintQueue(LinkQueue *Q) {
    printf("链队列（头->尾）: HEAD");
    QNode *p = Q->front->next;
    while (p != NULL) {
        printf(" -> %d", p->data);
        p = p->next;
    }
    printf(" -> NULL\n");
}

/* 销毁链队列，释放所有内存 */
void DestroyQueue(LinkQueue *Q) {
    int x;
    while (!QueueEmpty(Q)) DeQueue(Q, &x);
    free(Q->front);     /* 释放头结点 */
    Q->front = Q->rear = NULL;
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 链队列基本操作演示 =====\n\n");

    LinkQueue Q;
    InitQueue(&Q);

    printf("--- 入队操作 ---\n");
    for (int i = 1; i <= 5; i++) {
        EnQueue(&Q, i * 10);
        PrintQueue(&Q);
    }

    int head;
    GetHead(&Q, &head);
    printf("\n队头元素 = %d\n", head);

    printf("\n--- 出队操作 ---\n");
    int x;
    while (!QueueEmpty(&Q)) {
        DeQueue(&Q, &x);
        printf("出队 %d -> ", x);
        PrintQueue(&Q);
    }

    DestroyQueue(&Q);
    return 0;
}
