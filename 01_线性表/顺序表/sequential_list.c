/*
 * 顺序表（Sequential List）实现
 * 考研408数据结构 - 线性表
 *
 * 顺序表使用连续内存存储，支持随机访问
 * 时间复杂度：查找O(1)，插入/删除O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 50      /* 顺序表最大容量 */
#define InitSize 10     /* 动态顺序表初始容量 */

/* ===== 静态顺序表 ===== */
typedef struct {
    int data[MaxSize];  /* 顺序表元素数组 */
    int length;         /* 顺序表当前长度 */
} SeqList;

/* 初始化顺序表 */
void InitList(SeqList *L) {
    L->length = 0;      /* 初始长度为0 */
}

/* 销毁顺序表（静态版本无需释放内存） */
void DestroyList(SeqList *L) {
    L->length = 0;
}

/* 求顺序表长度 */
int Length(SeqList *L) {
    return L->length;
}

/* 判断顺序表是否为空 */
bool ListEmpty(SeqList *L) {
    return L->length == 0;
}

/* 打印顺序表所有元素 */
void PrintList(SeqList *L) {
    printf("顺序表（长度=%d）: [", L->length);
    for (int i = 0; i < L->length; i++) {
        printf("%d", L->data[i]);
        if (i < L->length - 1) printf(", ");
    }
    printf("]\n");
}

/*
 * 按位查找：获取第 i 个元素（1-based）
 * 时间复杂度：O(1)
 */
bool GetElem(SeqList *L, int i, int *e) {
    if (i < 1 || i > L->length) {
        printf("位置 %d 越界\n", i);
        return false;
    }
    *e = L->data[i - 1];   /* 注意：内部数组从0开始，对外接口从1开始 */
    return true;
}

/*
 * 按值查找：返回第一个值等于 e 的元素位置（1-based）
 * 时间复杂度：O(n)
 */
int LocateElem(SeqList *L, int e) {
    for (int i = 0; i < L->length; i++) {
        if (L->data[i] == e)
            return i + 1;   /* 返回1-based位置 */
    }
    return 0;   /* 未找到返回0 */
}

/*
 * 在位置 i 处插入元素 e（1-based）
 * 插入后原第i个及之后元素依次后移一位
 * 时间复杂度：O(n)，平均移动 n/2 个元素
 */
bool ListInsert(SeqList *L, int i, int e) {
    /* 检查位置合法性：1 <= i <= length+1 */
    if (i < 1 || i > L->length + 1) {
        printf("插入位置 %d 非法\n", i);
        return false;
    }
    /* 检查表满 */
    if (L->length >= MaxSize) {
        printf("顺序表已满，无法插入\n");
        return false;
    }
    /* 将第i个到第n个元素后移（从后往前移，防止覆盖） */
    for (int j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];
    }
    L->data[i - 1] = e;    /* 在位置i处放入新元素 */
    L->length++;            /* 表长加1 */
    return true;
}

/*
 * 删除位置 i 处的元素（1-based），并将被删元素值存入 e
 * 删除后原第i+1个及之后元素依次前移一位
 * 时间复杂度：O(n)，平均移动 (n-1)/2 个元素
 */
bool ListDelete(SeqList *L, int i, int *e) {
    /* 检查位置合法性 */
    if (i < 1 || i > L->length) {
        printf("删除位置 %d 非法\n", i);
        return false;
    }
    *e = L->data[i - 1];   /* 记录被删除元素 */
    /* 将第i+1个到第n个元素前移 */
    for (int j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j];
    }
    L->length--;            /* 表长减1 */
    return true;
}

/* ===== 动态顺序表（可扩容） ===== */
typedef struct {
    int *data;      /* 指向动态分配数组的指针 */
    int capacity;   /* 顺序表最大容量 */
    int length;     /* 顺序表当前长度 */
} DynSeqList;

/* 初始化动态顺序表 */
void InitDynList(DynSeqList *L) {
    L->data = (int *)malloc(InitSize * sizeof(int));
    if (!L->data) {
        printf("内存分配失败\n");
        exit(1);
    }
    L->capacity = InitSize;
    L->length = 0;
}

/* 动态顺序表扩容（扩容为原来的2倍） */
void IncreaseSize(DynSeqList *L) {
    int *p = L->data;
    L->data = (int *)realloc(L->data, 2 * L->capacity * sizeof(int));
    if (!L->data) {
        printf("扩容失败\n");
        L->data = p;    /* 恢复原指针 */
        return;
    }
    L->capacity *= 2;
    printf("顺序表扩容至 %d\n", L->capacity);
}

/* 动态顺序表插入 */
bool DynListInsert(DynSeqList *L, int i, int e) {
    if (i < 1 || i > L->length + 1) return false;
    if (L->length >= L->capacity) IncreaseSize(L);  /* 满了就扩容 */
    for (int j = L->length; j >= i; j--)
        L->data[j] = L->data[j - 1];
    L->data[i - 1] = e;
    L->length++;
    return true;
}

/* 销毁动态顺序表 */
void DestroyDynList(DynSeqList *L) {
    free(L->data);
    L->data = NULL;
    L->capacity = 0;
    L->length = 0;
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 顺序表基本操作演示 =====\n\n");

    SeqList L;
    InitList(&L);

    /* 插入元素 */
    printf("--- 插入操作 ---\n");
    for (int i = 1; i <= 5; i++) {
        ListInsert(&L, i, i * 10);  /* 插入10, 20, 30, 40, 50 */
    }
    PrintList(&L);

    /* 在第3位插入35 */
    ListInsert(&L, 3, 35);
    printf("在第3位插入35后: ");
    PrintList(&L);

    /* 按位查找 */
    printf("\n--- 查找操作 ---\n");
    int e;
    if (GetElem(&L, 3, &e))
        printf("第3个元素 = %d\n", e);

    /* 按值查找 */
    int pos = LocateElem(&L, 35);
    printf("值35在位置: %d\n", pos);

    pos = LocateElem(&L, 99);
    printf("值99在位置: %d（0表示未找到）\n", pos);

    /* 删除操作 */
    printf("\n--- 删除操作 ---\n");
    ListDelete(&L, 3, &e);
    printf("删除第3个元素（值=%d）后: ", e);
    PrintList(&L);

    /* 表长 */
    printf("\n当前表长 = %d\n", Length(&L));

    /* 动态顺序表演示 */
    printf("\n===== 动态顺序表扩容演示 =====\n");
    DynSeqList DL;
    InitDynList(&DL);
    for (int i = 1; i <= 12; i++) {
        DynListInsert(&DL, i, i);   /* 插入12个元素，超过初始容量10 */
    }
    printf("插入12个元素后，当前长度=%d，最大容量=%d\n", DL.length, DL.capacity);
    DestroyDynList(&DL);

    return 0;
}
