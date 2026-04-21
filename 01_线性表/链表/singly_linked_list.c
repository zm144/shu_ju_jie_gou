/*
 * 单链表（Singly Linked List）实现
 * 考研408数据结构 - 线性表
 *
 * 带头结点的单链表，头结点不存储数据
 * 头结点的存在使空表和非空表的操作统一
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 链表结点定义 */
typedef struct LNode {
    int data;               /* 数据域 */
    struct LNode *next;     /* 指针域，指向后继结点 */
} LNode, *LinkList;

/*
 * 初始化带头结点的单链表
 * 头结点 data 域不使用，next 指向第一个数据结点
 */
LinkList InitList(void) {
    LinkList L = (LNode *)malloc(sizeof(LNode));
    if (!L) { printf("内存分配失败\n"); exit(1); }
    L->next = NULL;     /* 头结点的next为NULL，表示空表 */
    return L;
}

/* 判断链表是否为空 */
bool ListEmpty(LinkList L) {
    return L->next == NULL;
}

/* 打印链表所有元素 */
void PrintList(LinkList L) {
    printf("链表: HEAD");
    LNode *p = L->next;
    while (p != NULL) {
        printf(" -> %d", p->data);
        p = p->next;
    }
    printf(" -> NULL\n");
}

/* 求链表长度 */
int Length(LinkList L) {
    int len = 0;
    LNode *p = L->next;
    while (p != NULL) {
        len++;
        p = p->next;
    }
    return len;
}

/*
 * 按位查找：获取第 i 个结点（1-based）
 * 时间复杂度：O(n)
 */
LNode *GetElem(LinkList L, int i) {
    if (i < 0) return NULL;
    if (i == 0) return L;   /* i=0返回头结点 */
    LNode *p = L->next;
    int j = 1;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p;   /* 若i超出范围，返回NULL */
}

/*
 * 按值查找：返回第一个值为 e 的结点指针
 * 时间复杂度：O(n)
 */
LNode *LocateElem(LinkList L, int e) {
    LNode *p = L->next;
    while (p != NULL && p->data != e)
        p = p->next;
    return p;   /* 未找到返回NULL */
}

/*
 * 在第 i 个位置插入元素 e（1-based）
 * 先找到第 i-1 个结点，再在其后插入新结点
 * 时间复杂度：O(n)
 */
bool ListInsert(LinkList L, int i, int e) {
    /* 找到第 i-1 个结点（i=1时找头结点） */
    LNode *p = GetElem(L, i - 1);
    if (p == NULL) {
        printf("插入位置 %d 非法\n", i);
        return false;
    }
    /* 创建新结点并插入 */
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if (!s) return false;
    s->data = e;
    s->next = p->next;  /* 新结点的next指向原第i个结点 */
    p->next = s;        /* 原第i-1个结点的next指向新结点 */
    return true;
}

/*
 * 删除第 i 个位置的结点（1-based），将其值存入 e
 * 时间复杂度：O(n)
 */
bool ListDelete(LinkList L, int i, int *e) {
    LNode *p = GetElem(L, i - 1);   /* 找到第i-1个结点 */
    if (p == NULL || p->next == NULL) {
        printf("删除位置 %d 非法\n", i);
        return false;
    }
    LNode *q = p->next;     /* q 为待删除结点 */
    *e = q->data;
    p->next = q->next;      /* 跳过q结点 */
    free(q);                /* 释放q结点内存 */
    return true;
}

/*
 * 头插法建立单链表
 * 输入序列为 a1, a2, ..., an，建立后链表顺序为 an -> ... -> a2 -> a1
 * 时间复杂度：O(n)
 */
LinkList CreateListHead(int *arr, int n) {
    LinkList L = InitList();
    for (int i = 0; i < n; i++) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        s->data = arr[i];
        s->next = L->next;  /* 新结点插到头结点后面 */
        L->next = s;
    }
    return L;   /* 结果链表与输入数组顺序相反 */
}

/*
 * 尾插法建立单链表
 * 输入序列为 a1, a2, ..., an，建立后链表顺序与输入相同
 * 时间复杂度：O(n)
 */
LinkList CreateListTail(int *arr, int n) {
    LinkList L = InitList();
    LNode *tail = L;        /* tail 始终指向链表尾结点 */
    for (int i = 0; i < n; i++) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        s->data = arr[i];
        s->next = NULL;
        tail->next = s;     /* 新结点插到尾部 */
        tail = s;           /* 更新尾指针 */
    }
    return L;
}

/*
 * 链表就地逆置（头插法）
 * 遍历原链表，每个结点依次用头插法插入新链表
 * 时间复杂度：O(n)，空间复杂度：O(1)
 *
 * ⭐ 考研高频考题
 */
void ReverseList(LinkList L) {
    LNode *p = L->next;     /* p 指向第一个数据结点 */
    L->next = NULL;         /* 断开头结点，相当于建立空链表 */
    while (p != NULL) {
        LNode *q = p->next; /* 保存下一个结点 */
        p->next = L->next;  /* 头插：将p插到头结点后 */
        L->next = p;
        p = q;              /* 继续处理下一个结点 */
    }
}

/*
 * 合并两个有序链表为一个有序链表
 * La 和 Lb 均为升序，合并后 La 为升序结果链表，Lb 变为空
 * 时间复杂度：O(m+n)
 *
 * ⭐ 考研常见题型
 */
void MergeSortedLists(LinkList La, LinkList Lb) {
    LNode *pa = La->next;   /* pa 遍历 La 的数据结点 */
    LNode *pb = Lb->next;   /* pb 遍历 Lb 的数据结点 */
    LNode *tail = La;       /* tail 指向结果链表的尾部，从头结点开始 */
    La->next = NULL;
    Lb->next = NULL;        /* 清空两个链表，重新建立 */

    while (pa != NULL && pb != NULL) {
        if (pa->data <= pb->data) {
            tail->next = pa;
            tail = pa;
            pa = pa->next;
        } else {
            tail->next = pb;
            tail = pb;
            pb = pb->next;
        }
    }
    /* 将剩余部分接到尾部 */
    tail->next = (pa != NULL) ? pa : pb;
}

/* 销毁链表，释放所有结点内存 */
void DestroyList(LinkList L) {
    LNode *p = L->next;
    while (p != NULL) {
        LNode *q = p->next;
        free(p);
        p = q;
    }
    free(L);    /* 释放头结点 */
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 单链表基本操作演示 =====\n\n");

    /* 尾插法建立链表 */
    int arr[] = {10, 20, 30, 40, 50};
    LinkList L = CreateListTail(arr, 5);
    printf("尾插法建立: ");
    PrintList(L);

    /* 头插法建立链表（顺序与输入相反） */
    LinkList L2 = CreateListHead(arr, 5);
    printf("头插法建立: ");
    PrintList(L2);
    DestroyList(L2);

    /* 插入操作 */
    printf("\n--- 插入操作 ---\n");
    ListInsert(L, 3, 25);
    printf("在第3位插入25: ");
    PrintList(L);

    /* 查找操作 */
    printf("\n--- 查找操作 ---\n");
    LNode *p = GetElem(L, 3);
    if (p) printf("第3个元素 = %d\n", p->data);
    p = LocateElem(L, 25);
    if (p) printf("值25对应结点地址有效\n");

    /* 删除操作 */
    printf("\n--- 删除操作 ---\n");
    int e;
    ListDelete(L, 3, &e);
    printf("删除第3个元素（值=%d）: ", e);
    PrintList(L);

    /* 链表逆置 */
    printf("\n--- 链表逆置（头插法）---\n");
    ReverseList(L);
    printf("逆置后: ");
    PrintList(L);

    /* 合并有序链表 */
    printf("\n--- 合并有序链表 ---\n");
    int a1[] = {1, 3, 5, 7};
    int a2[] = {2, 4, 6, 8};
    LinkList La = CreateListTail(a1, 4);
    LinkList Lb = CreateListTail(a2, 4);
    printf("La: "); PrintList(La);
    printf("Lb: "); PrintList(Lb);
    MergeSortedLists(La, Lb);
    printf("合并后: ");
    PrintList(La);
    DestroyList(La);
    free(Lb);   /* Lb头结点未在MergeSortedLists中释放 */

    printf("\n链表长度 = %d\n", Length(L));
    DestroyList(L);

    return 0;
}
