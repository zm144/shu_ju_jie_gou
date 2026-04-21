/*
 * 归并排序实现
 * 考研408数据结构 - 排序
 *
 * 实现：2路归并排序（递归版 + 迭代版）
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintArr(int *arr, int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

/*
 * 合并两个有序子数组
 * arr[left..mid] 和 arr[mid+1..right] 均已有序
 * 合并后 arr[left..right] 有序
 * 时间：O(n)，空间：O(n)（需要辅助数组）
 */
void Merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    /* 分配辅助数组 */
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    /* 复制到辅助数组 */
    memcpy(L, arr + left, n1 * sizeof(int));
    memcpy(R, arr + mid + 1, n2 * sizeof(int));

    /* 归并：依次比较L和R的元素，放回arr */
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        /* 相等时先取L（保证稳定性）*/
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    /* 将剩余部分复制回arr */
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

/*
 * 归并排序（递归版，自顶向下）
 * 时间：O(n log n)，空间：O(n)
 * 稳定 ✅
 */
void MergeSort(int *arr, int left, int right) {
    if (left >= right) return;  /* 子数组只有0或1个元素 */
    int mid = left + (right - left) / 2;
    MergeSort(arr, left, mid);          /* 递归排序左半 */
    MergeSort(arr, mid + 1, right);     /* 递归排序右半 */
    Merge(arr, left, mid, right);       /* 合并两半 */
}

/* 递归版（带过程输出） */
void MergeSortVerbose(int *arr, int left, int right, int depth) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    MergeSortVerbose(arr, left, mid, depth + 1);
    MergeSortVerbose(arr, mid + 1, right, depth + 1);

    for (int i = 0; i < depth; i++) printf("  ");
    printf("合并[%d..%d]和[%d..%d]: ", left, mid, mid+1, right);
    Merge(arr, left, mid, right);
    for (int i = left; i <= right; i++) printf("%d ", arr[i]);
    printf("\n");
}

/*
 * 归并排序（迭代版，自底向上）
 * 避免了递归调用栈，但逻辑稍复杂
 * 每次合并长度为size的相邻有序段（size从1开始翻倍）
 *
 * ⭐ 考研常考2路归并每趟结果
 */
void MergeSortIter(int *arr, int n) {
    for (int size = 1; size < n; size *= 2) {
        printf("  每段长度=%d: ", size);
        /* 依次合并每对相邻的有序段 */
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = left + 2 * size - 1;
            if (mid >= n) break;    /* 只有左半段，不需要合并 */
            if (right >= n) right = n - 1;  /* 右半段不完整 */
            Merge(arr, left, mid, right);
        }
        PrintArr(arr, n);
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    int original[] = {38, 27, 43, 3, 9, 82, 10};
    int n = 7;
    int arr[7];

    printf("原始数组: "); PrintArr(original, n);

    printf("\n--- 归并排序（递归，显示合并过程）---\n");
    for (int i = 0; i < n; i++) arr[i] = original[i];
    MergeSortVerbose(arr, 0, n - 1, 0);
    printf("结果: "); PrintArr(arr, n);

    printf("\n--- 归并排序（迭代2路归并，显示每趟结果）---\n");
    for (int i = 0; i < n; i++) arr[i] = original[i];
    printf("初始: "); PrintArr(arr, n);
    MergeSortIter(arr, n);
    printf("结果: "); PrintArr(arr, n);

    return 0;
}
