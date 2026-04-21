/*
 * 二叉搜索树（Binary Search Tree，BST）实现
 * 考研408数据结构 - 树
 *
 * BST性质：左子树所有值 < 根 < 右子树所有值
 * 中序遍历得到有序递增序列
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* BST结点定义 */
typedef struct BSTNode {
    int key;
    struct BSTNode *lchild;
    struct BSTNode *rchild;
} BSTNode, *BSTree;

/* 创建新结点 */
BSTNode *NewNode(int key) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    node->key = key;
    node->lchild = node->rchild = NULL;
    return node;
}

/*
 * BST查找（递归）
 * 时间复杂度：O(h)，h为树高
 */
BSTNode *BST_Search(BSTree T, int key) {
    if (T == NULL || T->key == key) return T;
    if (key < T->key)
        return BST_Search(T->lchild, key);  /* 小于根，查左子树 */
    else
        return BST_Search(T->rchild, key);  /* 大于根，查右子树 */
}

/* BST查找（迭代版，效率更高）*/
BSTNode *BST_SearchIter(BSTree T, int key) {
    while (T != NULL && T->key != key) {
        if (key < T->key) T = T->lchild;
        else T = T->rchild;
    }
    return T;
}

/*
 * BST插入
 * 递归找到合适位置插入新结点
 * 时间复杂度：O(h)
 */
BSTree BST_Insert(BSTree T, int key) {
    if (T == NULL) return NewNode(key); /* 找到空位，创建新结点 */
    if (key < T->key)
        T->lchild = BST_Insert(T->lchild, key);
    else if (key > T->key)
        T->rchild = BST_Insert(T->rchild, key);
    /* key == T->key 时，不重复插入 */
    return T;
}

/*
 * 找子树中最小结点（最左结点）
 */
BSTNode *FindMin(BSTree T) {
    while (T->lchild != NULL) T = T->lchild;
    return T;
}

/*
 * BST删除结点
 * 三种情况：
 * 1. 待删结点为叶结点：直接删除
 * 2. 待删结点只有一棵子树：用子树代替
 * 3. 待删结点有两棵子树：用中序后继（右子树最小值）替代，再删后继
 *
 * ⭐ 考研高频考题：删除有两个孩子的结点
 */
BSTree BST_Delete(BSTree T, int key) {
    if (T == NULL) return NULL;
    if (key < T->key) {
        T->lchild = BST_Delete(T->lchild, key);
    } else if (key > T->key) {
        T->rchild = BST_Delete(T->rchild, key);
    } else {
        /* 找到待删结点 */
        if (T->lchild == NULL) {
            /* 情况1和2：无左孩子，用右孩子替代 */
            BSTNode *tmp = T->rchild;
            free(T);
            return tmp;
        } else if (T->rchild == NULL) {
            /* 情况2：无右孩子，用左孩子替代 */
            BSTNode *tmp = T->lchild;
            free(T);
            return tmp;
        } else {
            /* 情况3：有两个孩子，找右子树最小结点（中序后继）替代 */
            BSTNode *successor = FindMin(T->rchild);
            T->key = successor->key;                        /* 用后继值替换 */
            T->rchild = BST_Delete(T->rchild, successor->key); /* 删除后继 */
        }
    }
    return T;
}

/* 中序遍历（输出有序序列）*/
void InOrder(BSTree T) {
    if (T == NULL) return;
    InOrder(T->lchild);
    printf("%d ", T->key);
    InOrder(T->rchild);
}

/* 打印BST结构（横向，便于观察） */
void PrintBST(BSTree T, int level) {
    if (T == NULL) return;
    PrintBST(T->rchild, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d\n", T->key);
    PrintBST(T->lchild, level + 1);
}

/* 销毁BST */
void DestroyBST(BSTree T) {
    if (T == NULL) return;
    DestroyBST(T->lchild);
    DestroyBST(T->rchild);
    free(T);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 二叉搜索树（BST）操作演示 =====\n\n");

    /* 依次插入：5, 3, 7, 1, 4, 6, 8 */
    int keys[] = {5, 3, 7, 1, 4, 6, 8};
    int n = 7;
    BSTree T = NULL;

    printf("插入序列: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
        T = BST_Insert(T, keys[i]);
    }
    printf("\n\nBST结构（逆时针90度）:\n");
    PrintBST(T, 0);

    printf("\n中序遍历（应为有序序列）: ");
    InOrder(T);
    printf("\n");

    /* 查找 */
    printf("\n--- 查找操作 ---\n");
    BSTNode *res = BST_Search(T, 4);
    printf("查找 4: %s\n", res ? "找到" : "未找到");
    res = BST_Search(T, 9);
    printf("查找 9: %s\n", res ? "找到" : "未找到");

    /* 删除叶结点 */
    printf("\n--- 删除操作 ---\n");
    printf("删除叶结点 1:\n");
    T = BST_Delete(T, 1);
    InOrder(T); printf("\n");

    /* 删除只有一个孩子的结点 */
    printf("删除只有一个孩子的结点 3:\n");
    T = BST_Delete(T, 3);
    InOrder(T); printf("\n");

    /* 删除有两个孩子的结点 */
    printf("删除有两个孩子的结点 5（根）:\n");
    T = BST_Delete(T, 5);
    InOrder(T); printf("\n");

    printf("\n删除后BST结构:\n");
    PrintBST(T, 0);

    DestroyBST(T);
    return 0;
}
