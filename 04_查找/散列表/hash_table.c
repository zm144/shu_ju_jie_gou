/*
 * 散列表（Hash Table）实现
 * 考研408数据结构 - 查找
 *
 * 实现：开放地址法（线性探测）和 链地址法（拉链法）
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ===== 开放地址法（线性探测）===== */

#define HASH_SIZE 11    /* 散列表大小（选质数） */
#define EMPTY     -1    /* 空槽标记 */
#define DELETED   -2    /* 已删除标记（开放地址法不能直接删除）*/

typedef struct {
    int table[HASH_SIZE];   /* 散列表数组 */
    int count;              /* 当前元素数 */
} OpenHashTable;

/* 初始化散列表 */
void InitOpenHash(OpenHashTable *H) {
    for (int i = 0; i < HASH_SIZE; i++)
        H->table[i] = EMPTY;
    H->count = 0;
}

/* 散列函数：除留余数法 */
int HashFunc(int key) {
    return key % HASH_SIZE;
}

/*
 * 开放地址法插入（线性探测）
 * 冲突时依次探测下一个位置
 * 时间复杂度：O(1) 平均，O(n) 最坏
 */
bool OpenHash_Insert(OpenHashTable *H, int key) {
    if (H->count >= HASH_SIZE) {
        printf("散列表已满\n");
        return false;
    }
    int h = HashFunc(key);
    int i = 0;
    while (i < HASH_SIZE) {
        int pos = (h + i) % HASH_SIZE;     /* 线性探测 */
        if (H->table[pos] == EMPTY || H->table[pos] == DELETED) {
            H->table[pos] = key;
            H->count++;
            printf("  插入%d到位置%d（探测%d次）\n", key, pos, i + 1);
            return true;
        }
        if (H->table[pos] == key) {
            printf("  键%d已存在\n", key);
            return false;
        }
        i++;
    }
    return false;
}

/*
 * 开放地址法查找
 */
int OpenHash_Search(OpenHashTable *H, int key) {
    int h = HashFunc(key);
    int cmp = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        int pos = (h + i) % HASH_SIZE;
        cmp++;
        if (H->table[pos] == EMPTY) break;  /* 遇到空槽，说明不存在 */
        if (H->table[pos] == key) {
            printf("  找到%d在位置%d（比较%d次）\n", key, pos, cmp);
            return pos;
        }
    }
    printf("  未找到%d（比较%d次）\n", key, cmp);
    return -1;
}

/*
 * 开放地址法删除（标记为DELETED，不能直接置空）
 * ⭐ 考研重点：开放地址法不能直接删除，需标记
 */
bool OpenHash_Delete(OpenHashTable *H, int key) {
    int h = HashFunc(key);
    for (int i = 0; i < HASH_SIZE; i++) {
        int pos = (h + i) % HASH_SIZE;
        if (H->table[pos] == EMPTY) return false;
        if (H->table[pos] == key) {
            H->table[pos] = DELETED;    /* 标记为已删除 */
            H->count--;
            return true;
        }
    }
    return false;
}

/* 打印散列表 */
void PrintOpenHash(OpenHashTable *H) {
    printf("散列表（开放地址法，大小=%d）:\n", HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("  [%2d]: ", i);
        if (H->table[i] == EMPTY)   printf("空\n");
        else if (H->table[i] == DELETED) printf("已删除\n");
        else printf("%d\n", H->table[i]);
    }
}

/* 计算开放地址法的平均查找长度（成功情况） */
double CalcASL(OpenHashTable *H, int *keys, int n) {
    int totalCmp = 0;
    for (int i = 0; i < n; i++) {
        int h = HashFunc(keys[i]);
        int cmp = 0;
        for (int j = 0; j < HASH_SIZE; j++) {
            int pos = (h + j) % HASH_SIZE;
            cmp++;
            if (H->table[pos] == keys[i]) break;
        }
        totalCmp += cmp;
    }
    return (double)totalCmp / n;
}

/* ===== 链地址法（拉链法）===== */

#define CHAIN_SIZE 7    /* 链地址法散列表大小 */

typedef struct ChainNode {
    int key;
    struct ChainNode *next;
} ChainNode;

typedef struct {
    ChainNode *table[CHAIN_SIZE];   /* 每个槽维护一个链表 */
    int count;
} ChainHashTable;

void InitChainHash(ChainHashTable *H) {
    for (int i = 0; i < CHAIN_SIZE; i++) H->table[i] = NULL;
    H->count = 0;
}

int ChainHashFunc(int key) { return key % CHAIN_SIZE; }

/* 链地址法插入 */
void ChainHash_Insert(ChainHashTable *H, int key) {
    int h = ChainHashFunc(key);
    /* 检查是否已存在 */
    ChainNode *p = H->table[h];
    while (p) { if (p->key == key) return; p = p->next; }
    /* 头插法 */
    ChainNode *node = (ChainNode *)malloc(sizeof(ChainNode));
    node->key = key;
    node->next = H->table[h];
    H->table[h] = node;
    H->count++;
}

/* 链地址法查找 */
bool ChainHash_Search(ChainHashTable *H, int key) {
    int h = ChainHashFunc(key);
    ChainNode *p = H->table[h];
    int cmp = 0;
    while (p) {
        cmp++;
        if (p->key == key) {
            printf("  找到%d（槽%d，比较%d次）\n", key, h, cmp);
            return true;
        }
        p = p->next;
    }
    printf("  未找到%d（槽%d，比较%d次）\n", key, h, cmp);
    return false;
}

/* 打印链地址法散列表 */
void PrintChainHash(ChainHashTable *H) {
    printf("散列表（链地址法，大小=%d）:\n", CHAIN_SIZE);
    for (int i = 0; i < CHAIN_SIZE; i++) {
        printf("  [%d]: ", i);
        ChainNode *p = H->table[i];
        while (p) { printf("%d -> ", p->key); p = p->next; }
        printf("NULL\n");
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 散列表演示 =====\n\n");

    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11};
    int n = 10;

    /* 开放地址法 */
    printf("--- 开放地址法（线性探测）---\n");
    printf("散列函数: h(key) = key %% %d\n", HASH_SIZE);
    printf("插入过程:\n");
    OpenHashTable OH;
    InitOpenHash(&OH);
    for (int i = 0; i < n; i++)
        OpenHash_Insert(&OH, keys[i]);

    PrintOpenHash(&OH);
    printf("装填因子 α = %d/%d = %.2f\n", OH.count, HASH_SIZE,
           (double)OH.count / HASH_SIZE);
    printf("ASL（成功）= %.2f\n", CalcASL(&OH, keys, n));

    printf("\n查找操作:\n");
    OpenHash_Search(&OH, 23);
    OpenHash_Search(&OH, 99);

    printf("\n删除55后再查找:\n");
    OpenHash_Delete(&OH, 55);
    OpenHash_Search(&OH, 11);   /* 11与55在同一探测序列上，验证删除标记的必要性 */

    /* 链地址法 */
    printf("\n--- 链地址法（拉链法）---\n");
    printf("散列函数: h(key) = key %% %d\n", CHAIN_SIZE);
    ChainHashTable CH;
    InitChainHash(&CH);
    for (int i = 0; i < n; i++)
        ChainHash_Insert(&CH, keys[i]);

    PrintChainHash(&CH);

    printf("\n查找操作:\n");
    ChainHash_Search(&CH, 23);
    ChainHash_Search(&CH, 99);

    return 0;
}
