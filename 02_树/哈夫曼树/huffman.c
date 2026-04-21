/*
 * 哈夫曼树（Huffman Tree）实现
 * 考研408数据结构 - 树
 *
 * 包含：构建哈夫曼树、生成哈夫曼编码、计算WPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100   /* 最大结点数（叶结点数的两倍） */
#define MAX_CODE  50    /* 哈夫曼编码最大长度 */

/* 哈夫曼树结点（顺序存储） */
typedef struct {
    int weight;     /* 权值 */
    int parent;     /* 父结点下标（-1表示无父结点，即根） */
    int lchild;     /* 左孩子下标（-1表示无） */
    int rchild;     /* 右孩子下标（-1表示无） */
} HuffNode;

/* 哈夫曼编码结构 */
typedef struct {
    char code[MAX_CODE];    /* 编码字符串（'0'和'1'组成） */
    int  length;            /* 编码长度 */
} HuffCode;

/*
 * 在 HuffTree[0..n-1] 中找到 parent==-1 且权值最小的两个结点下标
 * 时间复杂度：O(n)
 */
void SelectMin2(HuffNode *HT, int n, int *m1, int *m2) {
    *m1 = *m2 = -1;
    for (int i = 0; i < n; i++) {
        if (HT[i].parent != -1) continue;  /* 已有父结点，跳过 */
        if (*m1 == -1 || HT[i].weight < HT[*m1].weight) {
            *m2 = *m1;
            *m1 = i;
        } else if (*m2 == -1 || HT[i].weight < HT[*m2].weight) {
            *m2 = i;
        }
    }
}

/*
 * 构建哈夫曼树
 * 输入：n个叶结点的权值数组
 * 返回：哈夫曼树数组（大小2n-1），根结点在最后位置
 *
 * 时间复杂度：O(n²)（朴素选最小）
 */
HuffNode *BuildHuffmanTree(int *weights, int n) {
    int total = 2 * n - 1;  /* 哈夫曼树共2n-1个结点 */
    HuffNode *HT = (HuffNode *)malloc(total * sizeof(HuffNode));

    /* 初始化n个叶结点 */
    for (int i = 0; i < n; i++) {
        HT[i].weight = weights[i];
        HT[i].parent = HT[i].lchild = HT[i].rchild = -1;
    }
    /* 初始化n-1个内部结点 */
    for (int i = n; i < total; i++) {
        HT[i].weight = HT[i].parent = HT[i].lchild = HT[i].rchild = -1;
    }

    /* 合并n-1次 */
    for (int i = n; i < total; i++) {
        int m1, m2;
        SelectMin2(HT, i, &m1, &m2);   /* 在前i个结点中找最小两个 */
        /* 合并m1和m2为新结点i */
        HT[i].weight = HT[m1].weight + HT[m2].weight;
        HT[i].lchild = m1;
        HT[i].rchild = m2;
        HT[m1].parent = i;
        HT[m2].parent = i;
    }
    return HT;
}

/*
 * 计算带权路径长度 WPL
 * WPL = Σ(叶结点权值 × 从根到叶结点路径长度)
 */
int CalcWPL(HuffNode *HT, int n) {
    int wpl = 0;
    for (int i = 0; i < n; i++) {
        int depth = 0;
        int cur = i;
        /* 从叶结点往上数层数 */
        while (HT[cur].parent != -1) {
            cur = HT[cur].parent;
            depth++;
        }
        wpl += HT[i].weight * depth;
    }
    return wpl;
}

/*
 * 生成哈夫曼编码
 * 从每个叶结点向上回溯到根，记录路径（左0右1）再逆序
 */
HuffCode *GenHuffCodes(HuffNode *HT, int n) {
    HuffCode *codes = (HuffCode *)malloc(n * sizeof(HuffCode));
    char tmp[MAX_CODE];

    for (int i = 0; i < n; i++) {
        int len = 0;
        int cur = i;
        int par = HT[i].parent;
        /* 从叶结点往上，记录编码（逆序） */
        while (par != -1) {
            if (HT[par].lchild == cur)
                tmp[len++] = '0';   /* 当前是父结点的左孩子，编码0 */
            else
                tmp[len++] = '1';   /* 当前是父结点的右孩子，编码1 */
            cur = par;
            par = HT[par].parent;
        }
        /* 逆序存入codes[i] */
        codes[i].length = len;
        for (int j = 0; j < len; j++)
            codes[i].code[j] = tmp[len - 1 - j];
        codes[i].code[len] = '\0';
    }
    return codes;
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 哈夫曼树演示 =====\n\n");

    /* 字符及其频率（权值）*/
    char chars[] = {'a', 'b', 'c', 'd', 'e'};
    int weights[] = {5, 2, 9, 3, 7};
    int n = 5;

    printf("字符及权值:\n");
    for (int i = 0; i < n; i++)
        printf("  '%c': %d\n", chars[i], weights[i]);

    /* 构建哈夫曼树 */
    HuffNode *HT = BuildHuffmanTree(weights, n);
    int total = 2 * n - 1;

    printf("\n哈夫曼树结点信息（共%d个结点，前%d个为叶结点）:\n", total, n);
    printf("%-6s %-8s %-8s %-8s %-8s\n", "下标", "权值", "父结点", "左孩子", "右孩子");
    for (int i = 0; i < total; i++) {
        printf("%-6d %-8d %-8d %-8d %-8d", i, HT[i].weight,
               HT[i].parent, HT[i].lchild, HT[i].rchild);
        if (i < n) printf("  <- '%c'", chars[i]);
        printf("\n");
    }

    int wpl = CalcWPL(HT, n);
    printf("\n带权路径长度 WPL = %d\n", wpl);

    /* 生成编码 */
    HuffCode *codes = GenHuffCodes(HT, n);
    printf("\n哈夫曼编码:\n");
    for (int i = 0; i < n; i++) {
        printf("  '%c'(权值=%d): %s（长度=%d）\n",
               chars[i], weights[i], codes[i].code, codes[i].length);
    }

    /* 验证：编码0为左，1为右，从根往下验证 */
    printf("\n⭐ 验证：哈夫曼树有无度为1的结点:\n");
    int degree1 = 0;
    for (int i = n; i < total; i++) {
        if ((HT[i].lchild == -1) != (HT[i].rchild == -1))
            degree1++;
    }
    printf("度为1的结点数 = %d（应为0）\n", degree1);

    free(codes);
    free(HT);
    return 0;
}
