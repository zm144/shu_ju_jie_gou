/*
 * 交换排序实现
 * 考研408数据结构 - 排序
 *
 * 实现：冒泡排序 和 快速排序
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
 * 冒泡排序
 * 每趟将最大元素"冒泡"到末尾
 * 用flag优化：一趟无交换则数组已有序，提前退出
 *
 * 时间：最好O(n)，最坏O(n²)
 * 空间：O(1)
 * 稳定 ✅
 */
void BubbleSort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int flag = 0;   /* 标记本趟是否发生交换 */
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                Swap(&arr[j], &arr[j + 1]);
                flag = 1;
            }
        }
        if (!flag) break;   /* 无交换，已有序，提前退出 */
    }
}

/*
 * 快速排序的分区函数（Lomuto分区方案）
 * 选arr[high]为基准pivot
 * 将≤pivot的元素移到左边，>pivot的移到右边
 * 返回pivot最终位置
 *
 * ⭐ 考研常考：划分过程
 */
int Partition(int *arr, int low, int high) {
    int pivot = arr[high];  /* 选最后一个元素为基准 */
    int i = low - 1;        /* i指向小于pivot区间的末尾 */
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            Swap(&arr[i], &arr[j]);
        }
    }
    Swap(&arr[i + 1], &arr[high]);  /* pivot放到最终位置 */
    return i + 1;
}

/*
 * 经典双指针分区（Hoare方案，考研常用）
 * low从左往右找大于pivot的，high从右往左找小于pivot的，交换
 */
int PartitionHoare(int *arr, int low, int high) {
    int pivot = arr[low];   /* 选第一个元素为基准 */
    while (low < high) {
        while (low < high && arr[high] >= pivot) high--;
        arr[low] = arr[high];   /* 将小于pivot的元素移到左边 */
        while (low < high && arr[low] <= pivot) low++;
        arr[high] = arr[low];   /* 将大于pivot的元素移到右边 */
    }
    arr[low] = pivot;   /* pivot放到最终位置（low==high） */
    return low;
}

/*
 * 快速排序（递归）
 * 时间：最好/平均O(n log n)，最坏O(n²)（已有序）
 * 空间：O(log n) 平均（递归栈深度）
 * 不稳定 ❌
 */
void QuickSort(int *arr, int low, int high) {
    if (low >= high) return;
    int pivot = PartitionHoare(arr, low, high);  /* 划分 */
    QuickSort(arr, low, pivot - 1);     /* 递归排序左半部分 */
    QuickSort(arr, pivot + 1, high);    /* 递归排序右半部分 */
}

/* 演示快排的划分过程 */
void QuickSortVerbose(int *arr, int low, int high, int depth) {
    if (low >= high) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("排序[%d..%d]: ", low, high);
    for (int i = low; i <= high; i++) printf("%d ", arr[i]);
    printf("\n");

    int pivot = PartitionHoare(arr, low, high);
    for (int i = 0; i < depth; i++) printf("  ");
    printf("基准=%d放在位置%d\n", arr[pivot], pivot);

    QuickSortVerbose(arr, low, pivot - 1, depth + 1);
    QuickSortVerbose(arr, pivot + 1, high, depth + 1);
}

/* ===== 主函数演示 ===== */
int main(void) {
    int original[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    int arr[7];

    printf("原始数组: "); PrintArr(original, n);

    printf("\n--- 冒泡排序 ---\n");
    CopyArr(arr, original, n);
    BubbleSort(arr, n);
    printf("结果: "); PrintArr(arr, n);

    printf("\n--- 快速排序（显示过程）---\n");
    CopyArr(arr, original, n);
    printf("排序过程:\n");
    QuickSortVerbose(arr, 0, n - 1, 0);
    printf("结果: "); PrintArr(arr, n);

    /* 验证已有序数组的最坏情况 */
    printf("\n--- 快速排序（最坏情况验证）---\n");
    int sorted[] = {1, 2, 3, 4, 5};
    int m = 5;
    printf("已有序数组: "); PrintArr(sorted, m);
    printf("快速排序（此时退化为O(n²)）:\n");
    QuickSortVerbose(sorted, 0, m - 1, 0);

    return 0;
}
