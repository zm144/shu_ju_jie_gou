/*
 * 选择排序实现
 * 考研408数据结构 - 排序
 *
 * 实现：简单选择排序 和 堆排序
 */

#include <stdio.h>
#include <stdlib.h>

void PrintArr(int *arr, int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

void Swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void CopyArr(int *dst, int *src, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

/*
 * 简单选择排序
 * 每趟找到[i..n-1]中最小元素，与arr[i]交换
 *
 * 时间：O(n²)（无论输入，比较次数固定为n(n-1)/2）
 * 空间：O(1)
 * 不稳定 ❌（如：[3a, 3b, 1]，选1与3a交换后3a,3b顺序改变）
 *
 * ⭐ 简单选择排序比较次数固定，但移动次数最少（最多n-1次）
 */
void SelectionSort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        /* 找[i+1..n-1]中最小元素的下标 */
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i)
            Swap(&arr[i], &arr[minIdx]);    /* 将最小元素换到位置i */
    }
}

/*
 * 向下调整（Sift Down）：维护大根堆性质
 * 以i为根，在[i..heapSize-1]范围内向下调整
 * 时间复杂度：O(log n)
 *
 * ⭐ 堆排序的核心操作
 */
void SiftDown(int *arr, int i, int heapSize) {
    int largest = i;
    int left = 2 * i + 1;   /* 左孩子（0-indexed） */
    int right = 2 * i + 2;  /* 右孩子 */

    /* 找到父结点、左孩子、右孩子中最大的 */
    if (left < heapSize && arr[left] > arr[largest])
        largest = left;
    if (right < heapSize && arr[right] > arr[largest])
        largest = right;

    /* 若最大不是父结点，交换并继续向下调整 */
    if (largest != i) {
        Swap(&arr[i], &arr[largest]);
        SiftDown(arr, largest, heapSize);   /* 递归调整 */
    }
}

/*
 * 堆排序
 * 步骤1：建堆（O(n)）——从最后一个非叶结点向上，依次向下调整
 * 步骤2：排序（O(n log n)）——每次将堆顶（最大值）与末尾交换，堆大小减1，再调整堆顶
 *
 * 时间：O(n log n)（无论最好最坏）
 * 空间：O(1)（原地排序）
 * 不稳定 ❌
 *
 * ⭐ 建堆时间O(n)，整体O(n log n)
 */
void HeapSort(int *arr, int n) {
    /* 步骤1：建大根堆 */
    /* 从最后一个非叶结点（下标n/2-1）开始，向上依次调整 */
    for (int i = n / 2 - 1; i >= 0; i--) {
        SiftDown(arr, i, n);
    }
    printf("建堆完成（大根堆）: "); PrintArr(arr, n);

    /* 步骤2：排序：将堆顶（最大值）与最后元素交换，堆大小减1 */
    for (int i = n - 1; i > 0; i--) {
        Swap(&arr[0], &arr[i]);         /* 堆顶（最大值）换到末尾 */
        SiftDown(arr, 0, i);            /* 重新调整堆（堆大小减1） */
    }
}

/* 堆排序详细输出版本 */
void HeapSortVerbose(int *arr, int n) {
    /* 建堆 */
    printf("建堆过程:\n");
    for (int i = n / 2 - 1; i >= 0; i--) {
        printf("  调整结点%d: ", i);
        SiftDown(arr, i, n);
        PrintArr(arr, n);
    }

    printf("排序过程:\n");
    for (int i = n - 1; i > 0; i--) {
        printf("  堆顶%d与arr[%d]=%d交换: ", arr[0], i, arr[i]);
        Swap(&arr[0], &arr[i]);
        SiftDown(arr, 0, i);
        PrintArr(arr, n);
    }
}

/* 打印堆结构（树形） */
void PrintHeap(int *arr, int n) {
    printf("堆数组（完全二叉树）: ");
    PrintArr(arr, n);
    printf("父子关系（0-indexed）: 父i -> 左2i+1, 右2i+2\n");
}

/* ===== 主函数演示 ===== */
int main(void) {
    int original[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    int arr[7];

    printf("原始数组: "); PrintArr(original, n);

    printf("\n--- 简单选择排序 ---\n");
    CopyArr(arr, original, n);
    SelectionSort(arr, n);
    printf("结果: "); PrintArr(arr, n);

    printf("\n--- 堆排序（详细过程）---\n");
    CopyArr(arr, original, n);
    HeapSortVerbose(arr, n);
    printf("结果: "); PrintArr(arr, n);

    /* Top-K演示 */
    printf("\n--- 堆的Top-3应用（找最大3个元素）---\n");
    int data[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    int m = 9;
    printf("数据: "); PrintArr(data, m);
    /* 建大根堆，取3次堆顶 */
    for (int i = m / 2 - 1; i >= 0; i--) SiftDown(data, i, m);
    printf("Top-3最大元素: ");
    for (int k = 0; k < 3; k++) {
        printf("%d ", data[0]);
        Swap(&data[0], &data[m - 1 - k]);
        SiftDown(data, 0, m - 1 - k);
    }
    printf("\n");

    return 0;
}
