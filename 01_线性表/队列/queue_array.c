/*
 * 循环队列（Circular Queue）实现
 * 考研408数据结构 - 队列
 *
 * 用数组实现循环队列，解决普通顺序队列的"假溢出"问题
 * 牺牲一个存储单元来区分队空和队满
 *
 * ⭐ 核心：判空条件 front==rear，判满条件 (rear+1)%MaxSize==front
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 6   /* 循环队列最大容量（实际可存 MaxSize-1 个元素） */

/* 循环队列定义 */
typedef struct {
    int data[MaxSize];  /* 存储数组 */
    int front;          /* 队头指针，指向队头元素 */
    int rear;           /* 队尾指针，指向队尾元素的下一位置 */
} SqQueue;

/* 初始化循环队列 */
void InitQueue(SqQueue *Q) {
    Q->front = 0;
    Q->rear = 0;    /* front == rear 表示队空 */
}

/*
 * 判断队列是否为空
 * 队空条件：front == rear
 */
bool QueueEmpty(SqQueue *Q) {
    return Q->front == Q->rear;
}

/*
 * 判断队列是否已满
 * 队满条件：(rear+1) % MaxSize == front
 * 注意：牺牲了一个存储单元用于区分队空和队满
 *
 * ⭐ 考研必考：队满判断方法
 */
bool QueueFull(SqQueue *Q) {
    return (Q->rear + 1) % MaxSize == Q->front;
}

/*
 * 入队（EnQueue）：在队尾插入元素
 * 时间复杂度：O(1)
 */
bool EnQueue(SqQueue *Q, int x) {
    if (QueueFull(Q)) {
        printf("队满，无法入队\n");
        return false;
    }
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;  /* rear循环加1 */
    return true;
}

/*
 * 出队（DeQueue）：删除队头元素
 * 时间复杂度：O(1)
 */
bool DeQueue(SqQueue *Q, int *x) {
    if (QueueEmpty(Q)) {
        printf("队空，无法出队\n");
        return false;
    }
    *x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;  /* front循环加1 */
    return true;
}

/* 读取队头元素（不出队） */
bool GetHead(SqQueue *Q, int *x) {
    if (QueueEmpty(Q)) { printf("队空\n"); return false; }
    *x = Q->data[Q->front];
    return true;
}

/*
 * 获取队列中元素个数
 * 元素个数 = (rear - front + MaxSize) % MaxSize
 *
 * ⭐ 考研重点：循环队列求长度公式
 */
int QueueSize(SqQueue *Q) {
    return (Q->rear - Q->front + MaxSize) % MaxSize;
}

/* 打印队列元素（从队头到队尾） */
void PrintQueue(SqQueue *Q) {
    printf("队列（头->尾）[size=%d, front=%d, rear=%d]: [",
           QueueSize(Q), Q->front, Q->rear);
    int i = Q->front;
    bool first = true;
    while (i != Q->rear) {
        if (!first) printf(", ");
        printf("%d", Q->data[i]);
        i = (i + 1) % MaxSize;
        first = false;
    }
    printf("]\n");
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 循环队列基本操作演示 =====\n");
    printf("MaxSize=%d，最多存储 %d 个元素\n\n", MaxSize, MaxSize - 1);

    SqQueue Q;
    InitQueue(&Q);

    /* 入队操作 */
    printf("--- 入队操作 ---\n");
    for (int i = 1; i <= 5; i++) {
        EnQueue(&Q, i * 10);
        PrintQueue(&Q);
    }

    /* 尝试向已满的队列入队 */
    EnQueue(&Q, 60);

    /* 读队头 */
    int head;
    GetHead(&Q, &head);
    printf("\n队头元素 = %d\n", head);

    /* 出队操作 */
    printf("\n--- 出队操作 ---\n");
    int x;
    DeQueue(&Q, &x);
    printf("出队 %d -> ", x);
    PrintQueue(&Q);

    /* 入队后验证循环 */
    EnQueue(&Q, 60);
    printf("入队 60（利用循环空间）-> ");
    PrintQueue(&Q);

    /* 继续出队所有元素 */
    printf("\n--- 清空队列 ---\n");
    while (!QueueEmpty(&Q)) {
        DeQueue(&Q, &x);
        printf("出队: %d\n", x);
    }
    printf("队列已空: %s\n", QueueEmpty(&Q) ? "是" : "否");

    return 0;
}
