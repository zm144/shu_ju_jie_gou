/*
 * 双链表（Doubly Linked List）实现
 * 考研408数据结构 - 线性表
 *
 * 每个结点有两个指针：prior（前驱）和 next（后继）
 * 带头结点，支持双向遍历
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 双链表结点定义 */
typedef struct DNode {
    int data;               /* 数据域 */
    struct DNode *prior;    /* 前驱指针 */
    struct DNode *next;     /* 后继指针 */
} DNode, *DLinkList;

/* 初始化双链表（带头结点） */
DLinkList InitDList(void) {
    DLinkList L = (DNode *)malloc(sizeof(DNode));
    if (!L) { printf("内存分配失败\n"); exit(1); }
    L->prior = NULL;    /* 头结点的prior为NULL */
    L->next = NULL;     /* 头结点的next为NULL（空表） */
    return L;
}

/* 打印双链表（正向） */
void PrintDList(DLinkList L) {
    printf("双链表(正向): HEAD");
    DNode *p = L->next;
    while (p != NULL) {
        printf(" <-> %d", p->data);
        p = p->next;
    }
    printf(" <-> NULL\n");
}

/* 打印双链表（逆向）*/
void PrintDListReverse(DLinkList L) {
    /* 先找到尾结点 */
    DNode *p = L;
    while (p->next != NULL) p = p->next;
    /* 从尾往头逆向打印 */
    printf("双链表(逆向): NULL");
    while (p != L) {
        printf(" <-> %d", p->data);
        p = p->prior;
    }
    printf(" <-> HEAD\n");
}

/* 求双链表长度 */
int DListLength(DLinkList L) {
    int len = 0;
    DNode *p = L->next;
    while (p != NULL) { len++; p = p->next; }
    return len;
}

/*
 * 在结点 p 之后插入结点 s
 * 双链表插入需修改4个指针
 * 时间复杂度：O(1)
 *
 * 操作顺序很重要：先设置新结点s的指针，再修改p和q的指针
 */
bool InsertAfterNode(DNode *p, DNode *s) {
    if (!p || !s) return false;
    s->next = p->next;          /* ① s的next指向p的后继 */
    if (p->next != NULL)
        p->next->prior = s;     /* ② p的后继的prior改为s */
    s->prior = p;               /* ③ s的prior指向p */
    p->next = s;                /* ④ p的next改为s */
    return true;
}

/*
 * 按位插入：在第 i 个位置插入元素 e（1-based）
 */
bool DListInsert(DLinkList L, int i, int e) {
    /* 找到第 i-1 个结点 */
    DNode *p = L;
    int j = 0;
    while (p->next != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (j != i - 1 && i != 1) {
        printf("插入位置 %d 非法\n", i);
        return false;
    }
    DNode *s = (DNode *)malloc(sizeof(DNode));
    if (!s) return false;
    s->data = e;
    return InsertAfterNode(p, s);
}

/*
 * 删除结点 p（直接删除，不需要找前驱）
 * 双链表优势：可直接获取前驱，删除只需O(1)
 * 时间复杂度：O(1)
 *
 * ⭐ 考研重点：双链表删除结点的4步操作
 */
bool DeleteNode(DNode *p) {
    if (!p) return false;
    if (p->prior != NULL)
        p->prior->next = p->next;   /* 前驱的next绕过p */
    if (p->next != NULL)
        p->next->prior = p->prior;  /* 后继的prior绕过p */
    free(p);
    return true;
}

/*
 * 按位删除：删除第 i 个位置的元素（1-based）
 */
bool DListDelete(DLinkList L, int i, int *e) {
    DNode *p = L->next;
    int j = 1;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    if (p == NULL) {
        printf("删除位置 %d 非法\n", i);
        return false;
    }
    *e = p->data;
    DeleteNode(p);
    return true;
}

/* 尾插法建立双链表 */
DLinkList CreateDList(int *arr, int n) {
    DLinkList L = InitDList();
    DNode *tail = L;
    for (int i = 0; i < n; i++) {
        DNode *s = (DNode *)malloc(sizeof(DNode));
        s->data = arr[i];
        s->next = NULL;
        s->prior = tail;
        tail->next = s;
        tail = s;
    }
    return L;
}

/* 销毁双链表 */
void DestroyDList(DLinkList L) {
    DNode *p = L->next;
    while (p != NULL) {
        DNode *q = p->next;
        free(p);
        p = q;
    }
    free(L);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 双链表基本操作演示 =====\n\n");

    int arr[] = {10, 20, 30, 40, 50};
    DLinkList L = CreateDList(arr, 5);

    printf("初始链表:\n");
    PrintDList(L);
    PrintDListReverse(L);

    /* 插入操作 */
    printf("\n--- 插入操作 ---\n");
    DListInsert(L, 3, 25);
    printf("在第3位插入25:\n");
    PrintDList(L);

    /* 直接在某结点后插入 */
    DNode *p = L->next->next;  /* 指向第2个数据结点（值=20） */
    DNode *s = (DNode *)malloc(sizeof(DNode));
    s->data = 15;
    InsertAfterNode(p, s);
    printf("在20后插入15:\n");
    PrintDList(L);

    /* 删除操作 */
    printf("\n--- 删除操作 ---\n");
    int e;
    DListDelete(L, 3, &e);
    printf("删除第3个元素（值=%d）:\n", e);
    PrintDList(L);

    printf("\n当前双链表长度 = %d\n", DListLength(L));

    DestroyDList(L);
    printf("\n双链表已销毁\n");
    return 0;
}
