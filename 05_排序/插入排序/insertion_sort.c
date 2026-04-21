/*
 * 插入排序实现
 * 考研408数据结构 - 排序
 *
 * 实现：直接插入排序、折半插入排序、希尔排序
 */

#include <stdio.h>
#include <stdlib.h>

/* 打印数组 */
void PrintArr(int *arr, int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

/* 复制数组 */
void CopyArr(int *dst, int *src, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

/*
 * 直接插入排序
 * 将arr[i]插入到arr[0..i-1]的有序序列中的正确位置
 *
 * 时间：最好O(n)，最坏O(n²)，平均O(n²)
 * 空间：O(1)
 * 稳定 ✅
 */
void InsertionSort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];   /* 待插入元素 */
        int j = i - 1;
        /* 将大于key的元素依次后移 */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;   /* 插入到正确位置 */
    }
}

/*
 * 折半插入排序
 * 查找插入位置时用折半查找（减少比较次数，不减少移动次数）
 *
 * 时间：O(n²)（移动次数与直接插入相同）
 * 空间：O(1)
 * 稳定 ✅
 */
void BinaryInsertionSort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        /* 折半查找插入位置 */
        int low = 0, high = i - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] > key) high = mid - 1;
            else low = mid + 1;
        }
        /* low即为插入位置，将[low..i-1]后移一位 */
        for (int j = i - 1; j >= low; j--)
            arr[j + 1] = arr[j];
        arr[low] = key;
    }
}

/*
 * 希尔排序（缩小增量排序）
 * 按增量序列分组，对每组做直接插入排序
 * 增量从大到小，最后增量=1（一次完整插入排序）
 *
 * 时间：O(n^1.3) 平均
 * 空间：O(1)
 * 不稳定 ❌（跨组交换可能改变相同元素的相对顺序）
 *
 * ⭐ 希尔排序是唯一不稳定的插入类排序
 */
void ShellSort(int *arr, int n) {
    /* 增量序列：n/2, n/4, ..., 1 */
    for (int gap = n / 2; gap > 0; gap /= 2) {
        printf("  增量=%d: ", gap);
        /* 对每组做插入排序（步长为gap） */
        for (int i = gap; i < n; i++) {
            int key = arr[i];
            int j = i - gap;
            while (j >= 0 && arr[j] > key) {
                arr[j + gap] = arr[j];
                j -= gap;
            }
            arr[j + gap] = key;
        }
        PrintArr(arr, n);
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    int original[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    int arr[7];

    printf("原始数组: "); PrintArr(original, n);

    printf("\n--- 直接插入排序 ---\n");
    CopyArr(arr, original, n);
    InsertionSort(arr, n);
    printf("结果: "); PrintArr(arr, n);

    printf("\n--- 折半插入排序 ---\n");
    CopyArr(arr, original, n);
    BinaryInsertionSort(arr, n);
    printf("结果: "); PrintArr(arr, n);

    printf("\n--- 希尔排序 ---\n");
    CopyArr(arr, original, n);
    printf("排序过程:\n");
    ShellSort(arr, n);
    printf("结果: "); PrintArr(arr, n);

    return 0;
}
