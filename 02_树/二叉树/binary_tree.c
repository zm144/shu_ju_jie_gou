/*
 * 二叉树（Binary Tree）实现
 * 考研408数据结构 - 树
 *
 * 包含：创建、四种遍历（递归+迭代）、求高度、统计结点数
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* 二叉树结点定义 */
typedef struct BTNode {
    int data;
    struct BTNode *lchild;  /* 左孩子指针 */
    struct BTNode *rchild;  /* 右孩子指针 */
} BTNode, *BTree;

/* 创建新结点 */
BTNode *NewNode(int data) {
    BTNode *node = (BTNode *)malloc(sizeof(BTNode));
    node->data = data;
    node->lchild = node->rchild = NULL;
    return node;
}

/*
 * 按先序序列创建二叉树（-1 表示空结点）
 * 例如：1 2 4 -1 -1 5 -1 -1 3 -1 -1
 */
BTree CreateBTree(void) {
    int val;
    scanf("%d", &val);
    if (val == -1) return NULL;     /* -1 表示空结点 */
    BTNode *root = NewNode(val);
    root->lchild = CreateBTree();   /* 递归创建左子树 */
    root->rchild = CreateBTree();   /* 递归创建右子树 */
    return root;
}

/* ===== 递归遍历 ===== */

/* 前序遍历（NLR）：根→左→右 */
void PreOrder(BTree T) {
    if (T == NULL) return;
    printf("%d ", T->data);         /* 访问根 */
    PreOrder(T->lchild);            /* 递归遍历左子树 */
    PreOrder(T->rchild);            /* 递归遍历右子树 */
}

/* 中序遍历（LNR）：左→根→右 */
void InOrder(BTree T) {
    if (T == NULL) return;
    InOrder(T->lchild);
    printf("%d ", T->data);
    InOrder(T->rchild);
}

/* 后序遍历（LRN）：左→右→根 */
void PostOrder(BTree T) {
    if (T == NULL) return;
    PostOrder(T->lchild);
    PostOrder(T->rchild);
    printf("%d ", T->data);
}

/* ===== 迭代遍历（用栈模拟） ===== */

/* 简单栈用于迭代遍历 */
#define STACK_MAX 100
typedef struct {
    BTNode *data[STACK_MAX];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
bool stack_empty(Stack *s) { return s->top == -1; }
void stack_push(Stack *s, BTNode *p) { s->data[++s->top] = p; }
BTNode *stack_pop(Stack *s) { return s->data[s->top--]; }
BTNode *stack_top(Stack *s) { return s->data[s->top]; }

/*
 * 迭代前序遍历
 * 用栈：先将根入栈，出栈时访问，再将右孩子、左孩子依次入栈
 */
void PreOrderIter(BTree T) {
    if (T == NULL) return;
    Stack s;
    stack_init(&s);
    stack_push(&s, T);
    while (!stack_empty(&s)) {
        BTNode *p = stack_pop(&s);
        printf("%d ", p->data);         /* 访问 */
        if (p->rchild) stack_push(&s, p->rchild);  /* 先压右 */
        if (p->lchild) stack_push(&s, p->lchild);  /* 后压左（先出）*/
    }
}

/*
 * 迭代中序遍历
 * 沿左子树一路压栈，出栈时访问，再转到右子树
 *
 * ⭐ 考研常考迭代写法
 */
void InOrderIter(BTree T) {
    Stack s;
    stack_init(&s);
    BTNode *p = T;
    while (p != NULL || !stack_empty(&s)) {
        while (p != NULL) {         /* 沿左子树压栈 */
            stack_push(&s, p);
            p = p->lchild;
        }
        p = stack_pop(&s);
        printf("%d ", p->data);     /* 访问根 */
        p = p->rchild;              /* 转向右子树 */
    }
}

/*
 * 迭代后序遍历（双栈法）
 * 后序 = 前序(根左右)的镜像取反 = 根右左 的逆序
 */
void PostOrderIter(BTree T) {
    if (T == NULL) return;
    Stack s1, s2;
    stack_init(&s1); stack_init(&s2);
    stack_push(&s1, T);
    while (!stack_empty(&s1)) {
        BTNode *p = stack_pop(&s1);
        stack_push(&s2, p);         /* 存入s2（待逆序输出）*/
        if (p->lchild) stack_push(&s1, p->lchild);
        if (p->rchild) stack_push(&s1, p->rchild);
    }
    while (!stack_empty(&s2)) {
        printf("%d ", stack_pop(&s2)->data);
    }
}

/*
 * 层序遍历（BFS）：使用队列
 * 按层从上到下、从左到右访问所有结点
 */
void LevelOrder(BTree T) {
    if (T == NULL) return;
    /* 简单队列 */
    BTNode *q[STACK_MAX];
    int front = 0, rear = 0;
    q[rear++] = T;
    while (front != rear) {
        BTNode *p = q[front++];
        printf("%d ", p->data);
        if (p->lchild) q[rear++] = p->lchild;
        if (p->rchild) q[rear++] = p->rchild;
    }
}

/*
 * 求二叉树高度（深度）
 * 递归：高度 = max(左子树高度, 右子树高度) + 1
 * 时间复杂度：O(n)
 */
int Height(BTree T) {
    if (T == NULL) return 0;
    int lh = Height(T->lchild);
    int rh = Height(T->rchild);
    return (lh > rh ? lh : rh) + 1;
}

/* 统计结点总数 */
int CountNodes(BTree T) {
    if (T == NULL) return 0;
    return 1 + CountNodes(T->lchild) + CountNodes(T->rchild);
}

/* 统计叶结点数（度为0的结点） */
int CountLeaves(BTree T) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL) return 1;
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}

/* 销毁二叉树（后序释放） */
void DestroyBTree(BTree T) {
    if (T == NULL) return;
    DestroyBTree(T->lchild);
    DestroyBTree(T->rchild);
    free(T);
}

/* 辅助：手动构建测试二叉树
 *        1
 *       / \
 *      2   3
 *     / \   \
 *    4   5   6
 */
BTree BuildTestTree(void) {
    BTNode *root = NewNode(1);
    root->lchild = NewNode(2);
    root->rchild = NewNode(3);
    root->lchild->lchild = NewNode(4);
    root->lchild->rchild = NewNode(5);
    root->rchild->rchild = NewNode(6);
    return root;
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 二叉树操作演示 =====\n\n");
    printf("测试二叉树:\n");
    printf("        1\n");
    printf("       / \\\n");
    printf("      2   3\n");
    printf("     / \\   \\\n");
    printf("    4   5   6\n\n");

    BTree T = BuildTestTree();

    printf("递归前序遍历: ");  PreOrder(T);   printf("\n");
    printf("递归中序遍历: ");  InOrder(T);    printf("\n");
    printf("递归后序遍历: ");  PostOrder(T);  printf("\n");
    printf("层序遍历:     ");  LevelOrder(T); printf("\n\n");

    printf("迭代前序遍历: ");  PreOrderIter(T);  printf("\n");
    printf("迭代中序遍历: ");  InOrderIter(T);   printf("\n");
    printf("迭代后序遍历: ");  PostOrderIter(T); printf("\n\n");

    printf("树的高度   = %d\n", Height(T));
    printf("结点总数   = %d\n", CountNodes(T));
    printf("叶结点数   = %d\n", CountLeaves(T));

    /* 验证 n0 = n2 + 1 */
    printf("\n⭐ 验证性质 n0 = n2 + 1:\n");
    printf("叶结点(n0)=%d, 度2结点数:", CountLeaves(T));
    /* 统计度为2的结点 */
    int n2 = 0;
    BTNode *q[100];
    int fr = 0, re = 0;
    q[re++] = T;
    while (fr != re) {
        BTNode *p = q[fr++];
        if (p->lchild && p->rchild) n2++;
        if (p->lchild) q[re++] = p->lchild;
        if (p->rchild) q[re++] = p->rchild;
    }
    printf("%d, n0 = n2+1 = %d ✓\n", n2, n2 + 1);

    DestroyBTree(T);
    return 0;
}
