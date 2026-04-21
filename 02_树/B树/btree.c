/*
 * B树（B-Tree）简化实现
 * 考研408数据结构 - 树
 *
 * 实现3阶B树（2-3树）的查找和插入操作
 * 每个结点最多2个关键字，最多3个孩子
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 3             /* B树阶数 */
#define MAX_KEYS (M-1)  /* 每个结点最多关键字数 */
#define MIN_KEYS ((M+1)/2 - 1)  /* 非根非叶结点最少关键字数 */

/* B树结点 */
typedef struct BTreeNode {
    int keys[M];                    /* 关键字数组（最多M-1个） */
    struct BTreeNode *children[M+1];/* 孩子指针数组（最多M个） */
    int n;                          /* 当前关键字数量 */
    bool leaf;                      /* 是否为叶结点 */
} BTreeNode;

/* 创建新结点 */
BTreeNode *NewBTreeNode(bool leaf) {
    BTreeNode *node = (BTreeNode *)calloc(1, sizeof(BTreeNode));
    node->leaf = leaf;
    node->n = 0;
    return node;
}

/*
 * B树查找
 * 在以T为根的子树中查找关键字key
 * 返回含key的结点，并通过*idx返回在结点中的位置
 */
BTreeNode *BTree_Search(BTreeNode *T, int key, int *idx) {
    int i = 0;
    /* 找到第一个 >= key 的位置 */
    while (i < T->n && key > T->keys[i]) i++;

    if (i < T->n && key == T->keys[i]) {
        *idx = i;
        return T;   /* 在当前结点找到 */
    }
    if (T->leaf) return NULL;   /* 叶结点中未找到 */

    return BTree_Search(T->children[i], key, idx);  /* 递归查找孩子 */
}

/*
 * 对孩子结点 children[i] 进行分裂
 * 当children[i]已满（M-1个关键字）时，分裂为两个结点
 * 中间关键字上移至父结点
 */
void SplitChild(BTreeNode *parent, int i) {
    BTreeNode *full = parent->children[i];  /* 待分裂的满结点 */
    BTreeNode *newNode = NewBTreeNode(full->leaf);  /* 新结点 */
    int mid = MAX_KEYS / 2;     /* 中间位置 */

    /* 将满结点后半部分关键字移到新结点 */
    newNode->n = MAX_KEYS - mid - 1;
    for (int j = 0; j < newNode->n; j++)
        newNode->keys[j] = full->keys[mid + 1 + j];

    /* 如果不是叶结点，也移动孩子指针 */
    if (!full->leaf) {
        for (int j = 0; j <= newNode->n; j++)
            newNode->children[j] = full->children[mid + 1 + j];
    }

    full->n = mid;  /* 满结点保留前半部分 */

    /* 在父结点中为新结点腾出位置 */
    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = newNode;

    /* 中间关键字上移到父结点 */
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = full->keys[mid];
    parent->n++;
}

/*
 * 向非满结点中插入关键字
 */
void InsertNonFull(BTreeNode *T, int key) {
    int i = T->n - 1;
    if (T->leaf) {
        /* 叶结点：直接插入，保持有序 */
        while (i >= 0 && T->keys[i] > key) {
            T->keys[i + 1] = T->keys[i];
            i--;
        }
        T->keys[i + 1] = key;
        T->n++;
    } else {
        /* 内部结点：找到合适的孩子递归插入 */
        while (i >= 0 && T->keys[i] > key) i--;
        i++;
        if (T->children[i]->n == MAX_KEYS) {
            /* 孩子已满，先分裂 */
            SplitChild(T, i);
            /* 分裂后确定插入哪个孩子 */
            if (key > T->keys[i]) i++;
        }
        InsertNonFull(T->children[i], key);
    }
}

/*
 * B树插入
 * 若根结点已满，需先分裂根（树高增加）
 */
BTreeNode *BTree_Insert(BTreeNode *root, int key) {
    if (root->n == MAX_KEYS) {
        /* 根满了，创建新根，分裂旧根 */
        BTreeNode *newRoot = NewBTreeNode(false);
        newRoot->children[0] = root;
        SplitChild(newRoot, 0);
        InsertNonFull(newRoot, key);
        return newRoot;
    }
    InsertNonFull(root, key);
    return root;
}

/* 打印B树（层序） */
void PrintBTree(BTreeNode *T, int level) {
    if (!T) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("[");
    for (int i = 0; i < T->n; i++) {
        printf("%d", T->keys[i]);
        if (i < T->n - 1) printf(",");
    }
    printf("]%s\n", T->leaf ? "(leaf)" : "");
    if (!T->leaf) {
        for (int i = 0; i <= T->n; i++)
            PrintBTree(T->children[i], level + 1);
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== %d阶B树演示 =====\n", M);
    printf("每结点最多%d个关键字，最多%d个孩子\n\n", MAX_KEYS, M);

    BTreeNode *root = NewBTreeNode(true);   /* 初始根为叶结点 */

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = 8;

    for (int i = 0; i < n; i++) {
        printf("插入 %d:\n", keys[i]);
        root = BTree_Insert(root, keys[i]);
        PrintBTree(root, 0);
        printf("\n");
    }

    /* 查找 */
    int idx;
    BTreeNode *res = BTree_Search(root, 6, &idx);
    printf("查找 6: %s\n", res ? "找到" : "未找到");
    res = BTree_Search(root, 15, &idx);
    printf("查找 15: %s\n", res ? "找到" : "未找到");

    return 0;
}
