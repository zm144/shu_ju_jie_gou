/*
 * 查找算法实现
 * 考研408数据结构 - 查找
 *
 * 实现：顺序查找、折半查找、斐波那契查找
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 顺序查找（带哨兵优化）
 * 将目标值放在arr[0]作为哨兵，从后往前比较，省去越界判断
 * 时间复杂度：O(n)
 */
int SeqSearch(int *arr, int n, int key) {
    arr[0] = key;       /* 哨兵：放在arr[0]，保证一定能找到 */
    int i = n;
    while (arr[i] != key) i--;
    return i;           /* i==0 表示未找到，i>0 表示找到的位置（1-based） */
}

/*
 * 折半查找（二分查找）
 * 前提：arr是有序数组（升序）
 * 每次比较中间元素，排除一半
 * 时间复杂度：O(log n)
 *
 * ⭐ 考研重点：前提必须是有序顺序表
 */
int BinarySearch(int *arr, int n, int key) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;   /* 防溢出写法 */
        if (arr[mid] == key)
            return mid;             /* 找到，返回下标 */
        else if (arr[mid] < key)
            low = mid + 1;          /* 目标在右半部分 */
        else
            high = mid - 1;         /* 目标在左半部分 */
    }
    return -1;  /* 未找到 */
}

/*
 * 折半查找（递归版）
 */
int BinarySearchRecur(int *arr, int low, int high, int key) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid] == key) return mid;
    if (arr[mid] < key) return BinarySearchRecur(arr, mid + 1, high, key);
    return BinarySearchRecur(arr, low, mid - 1, key);
}

/*
 * 斐波那契查找
 * 利用斐波那契数列确定分割点（黄金分割比）
 * 每次分割的左半部分大小为F(k-1)-1，右半部分为F(k-2)-1
 * 时间复杂度：O(log n)
 */

/* 生成斐波那契数列 */
void GenFib(int *F, int maxLen) {
    F[0] = 1; F[1] = 1;
    for (int i = 2; i < maxLen; i++)
        F[i] = F[i-1] + F[i-2];
}

int FibSearch(int *arr, int n, int key) {
    int F[20];
    GenFib(F, 20);

    /* 找到第一个 F[k] >= n 的k */
    int k = 0;
    while (F[k] < n) k++;

    /* 构造扩展数组（长度扩展到F[k]，多余位用最后一个元素填充） */
    int *tmp = (int *)malloc((F[k] + 1) * sizeof(int));
    memcpy(tmp, arr, n * sizeof(int));
    for (int i = n; i < F[k]; i++)
        tmp[i] = arr[n - 1];    /* 用最后一个元素填充 */

    int low = 0, high = n - 1, result = -1;
    while (low <= high) {
        int mid = low + F[k - 1] - 1;  /* 斐波那契分割点 */
        if (mid > high) mid = high;
        if (key == tmp[mid]) {
            result = (mid < n) ? mid : n - 1;
            break;
        } else if (key < tmp[mid]) {
            high = mid - 1;
            k--;
        } else {
            low = mid + 1;
            k -= 2;
        }
    }
    free(tmp);
    return result;
}

/* 计算折半查找的平均查找长度（ASL） */
void CalcBinarySearchASL(int n) {
    /* 建立判定树，统计每个元素的比较次数 */
    printf("折半查找判定树分析（n=%d）:\n", n);
    double totalCmp = 0;
    int low = 0, high = n - 1;
    /* 模拟每个元素的查找过程 */
    for (int target = 0; target < n; target++) {
        int l = 0, h = n - 1, cmp = 0;
        while (l <= h) {
            int mid = l + (h - l) / 2;
            cmp++;
            if (mid == target) break;
            if (mid < target) l = mid + 1;
            else h = mid - 1;
        }
        totalCmp += cmp;
    }
    printf("  ASL_成功 = %.2f（理论≈%.2f）\n",
           totalCmp / n, (double)(n + 1) / n * 1.0);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 查找算法演示 =====\n\n");

    /* 有序数组 */
    int sorted[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    int n = 10;

    printf("有序数组: ");
    for (int i = 0; i < n; i++) printf("%d ", sorted[i]);
    printf("\n\n");

    /* 顺序查找 */
    printf("--- 顺序查找 ---\n");
    /* 顺序查找需要额外的哨兵位，所以用一个稍大的数组 */
    int arr_seq[12];
    arr_seq[0] = 0;  /* 哨兵位 */
    for (int i = 0; i < n; i++) arr_seq[i + 1] = sorted[i];
    int pos = SeqSearch(arr_seq, n, 25);
    printf("查找25: 位置=%d（1-based）\n", pos);
    pos = SeqSearch(arr_seq, n, 99);
    printf("查找99: 位置=%d（0表示未找到）\n", pos);

    /* 折半查找 */
    printf("\n--- 折半查找 ---\n");
    pos = BinarySearch(sorted, n, 25);
    printf("查找25: 下标=%d\n", pos);
    pos = BinarySearch(sorted, n, 30);
    printf("查找30: 下标=%d\n", pos);
    pos = BinarySearch(sorted, n, 99);
    printf("查找99: 下标=%d（-1表示未找到）\n", pos);

    /* 折半查找（递归） */
    printf("\n--- 折半查找（递归） ---\n");
    pos = BinarySearchRecur(sorted, 0, n - 1, 40);
    printf("查找40: 下标=%d\n", pos);

    /* 斐波那契查找 */
    printf("\n--- 斐波那契查找 ---\n");
    pos = FibSearch(sorted, n, 35);
    printf("查找35: 下标=%d\n", pos);
    pos = FibSearch(sorted, n, 99);
    printf("查找99: 下标=%d（-1表示未找到）\n", pos);

    /* 分析折半查找ASL */
    printf("\n");
    CalcBinarySearchASL(n);

    return 0;
}
