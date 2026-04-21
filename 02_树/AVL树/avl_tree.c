/*
 * AVL树（平衡二叉搜索树）实现
 * 考研408数据结构 - 树
 *
 * 包含四种旋转：LL（右旋）、RR（左旋）、LR（先左后右）、RL（先右后左）
 * 每次插入后自动调整保持平衡
 */

#include <stdio.h>
#include <stdlib.h>

/* AVL树结点定义 */
typedef struct AVLNode {
    int key;
    int height;                 /* 结点高度（叶结点高度为1） */
    struct AVLNode *lchild;
    struct AVLNode *rchild;
} AVLNode, *AVLTree;

/* 获取结点高度（空结点高度为0） */
int Height(AVLNode *node) {
    return node ? node->height : 0;
}

/* 获取两数中较大值 */
int Max(int a, int b) { return a > b ? a : b; }

/* 更新结点高度 */
void UpdateHeight(AVLNode *node) {
    node->height = Max(Height(node->lchild), Height(node->rchild)) + 1;
}

/* 获取平衡因子（左高-右高） */
int GetBalance(AVLNode *node) {
    return node ? Height(node->lchild) - Height(node->rchild) : 0;
}

/* 创建新结点 */
AVLNode *NewNode(int key) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    node->key = key;
    node->height = 1;   /* 新结点高度为1 */
    node->lchild = node->rchild = NULL;
    return node;
}

/*
 * LL旋转（右旋）：用于处理 LL 型失衡
 * 失衡结点 y 的左孩子 x 的左子树过高
 *
 *       y                  x
 *      / \               /   \
 *     x   T4    -->     z     y
 *    / \               / \   / \
 *   z   T3            T1 T2 T3 T4
 *  / \
 * T1  T2
 *
 * 操作：x成为新根，y成为x的右孩子，x原来的右子树T3变为y的左子树
 */
AVLNode *RotateRight(AVLNode *y) {
    AVLNode *x = y->lchild;
    AVLNode *T3 = x->rchild;

    x->rchild = y;      /* x的右孩子改为y */
    y->lchild = T3;     /* y的左孩子改为T3 */

    /* 先更新y的高度，再更新x的高度（y现在是x的子树） */
    UpdateHeight(y);
    UpdateHeight(x);

    return x;   /* x成为新根 */
}

/*
 * RR旋转（左旋）：用于处理 RR 型失衡
 * 失衡结点 y 的右孩子 x 的右子树过高
 *
 *     y                    x
 *    / \                 /   \
 *   T1   x      -->     y     z
 *       / \            / \   / \
 *      T2  z          T1 T2 T3 T4
 *         / \
 *        T3  T4
 */
AVLNode *RotateLeft(AVLNode *y) {
    AVLNode *x = y->rchild;
    AVLNode *T2 = x->lchild;

    x->lchild = y;
    y->rchild = T2;

    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

/*
 * LR旋转：用于处理 LR 型失衡
 * 先对左孩子做 RR 左旋，再对自身做 LL 右旋
 *
 *       z                  y
 *      / \               /   \
 *     x   T4   -->      x     z
 *    / \               / \   / \
 *   T1   y            T1 T2 T3 T4
 *       / \
 *      T2  T3
 */
AVLNode *RotateLR(AVLNode *z) {
    z->lchild = RotateLeft(z->lchild);   /* 先左旋左孩子 */
    return RotateRight(z);               /* 再右旋自身 */
}

/*
 * RL旋转：用于处理 RL 型失衡
 * 先对右孩子做 LL 右旋，再对自身做 RR 左旋
 */
AVLNode *RotateRL(AVLNode *z) {
    z->rchild = RotateRight(z->rchild);  /* 先右旋右孩子 */
    return RotateLeft(z);                /* 再左旋自身 */
}

/*
 * AVL树插入
 * 递归插入后，回溯时检查并修复失衡
 * 时间复杂度：O(log n)
 */
AVLTree AVL_Insert(AVLTree T, int key) {
    /* 1. 标准BST插入 */
    if (T == NULL) return NewNode(key);
    if (key < T->key)
        T->lchild = AVL_Insert(T->lchild, key);
    else if (key > T->key)
        T->rchild = AVL_Insert(T->rchild, key);
    else
        return T;   /* 重复key不插入 */

    /* 2. 更新当前结点高度 */
    UpdateHeight(T);

    /* 3. 检查平衡因子，决定旋转类型 */
    int balance = GetBalance(T);

    /* LL型：左子树更高，且插入在左孩子的左子树 */
    if (balance > 1 && key < T->lchild->key)
        return RotateRight(T);

    /* RR型：右子树更高，且插入在右孩子的右子树 */
    if (balance < -1 && key > T->rchild->key)
        return RotateLeft(T);

    /* LR型：左子树更高，但插入在左孩子的右子树 */
    if (balance > 1 && key > T->lchild->key)
        return RotateLR(T);

    /* RL型：右子树更高，但插入在右孩子的左子树 */
    if (balance < -1 && key < T->rchild->key)
        return RotateRL(T);

    return T;   /* 无需旋转，返回原根 */
}

/* 中序遍历（验证BST性质）*/
void InOrder(AVLTree T) {
    if (T == NULL) return;
    InOrder(T->lchild);
    printf("%d(h=%d,bf=%d) ", T->key, T->height, GetBalance(T));
    InOrder(T->rchild);
}

/* 打印AVL树结构 */
void PrintAVL(AVLTree T, int level) {
    if (T == NULL) return;
    PrintAVL(T->rchild, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d[h=%d]\n", T->key, T->height);
    PrintAVL(T->lchild, level + 1);
}

/* 销毁AVL树 */
void DestroyAVL(AVLTree T) {
    if (!T) return;
    DestroyAVL(T->lchild);
    DestroyAVL(T->rchild);
    free(T);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== AVL树操作演示 =====\n\n");

    AVLTree T = NULL;
    /* 插入序列 10, 20, 30（触发RR左旋），再插入5, 4（触发LL右旋），再插入15（触发LR）*/
    int keys[] = {10, 20, 30, 5, 4, 15};
    int n = 6;

    for (int i = 0; i < n; i++) {
        printf("插入 %d:\n", keys[i]);
        T = AVL_Insert(T, keys[i]);
        PrintAVL(T, 0);
        printf("中序: "); InOrder(T); printf("\n\n");
    }

    printf("最终树高 = %d\n", Height(T));
    printf("根结点 = %d, 平衡因子 = %d\n", T->key, GetBalance(T));

    DestroyAVL(T);
    return 0;
}
