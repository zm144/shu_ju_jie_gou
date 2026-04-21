/*
 * 基数排序（LSD Radix Sort）实现
 * 考研408数据结构 - 排序
 *
 * 从最低位到最高位，对每一位做计数排序
 * 时间复杂度：O(d*(n+r))，d=位数，r=基数（10进制则r=10）
 * 稳定 ✅
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintArr(int *arr, int n) {
    for (int i = 0; i < n; i++) printf("%3d ", arr[i]);
    printf("\n");
}

/* 获取数组最大值（用于确定最大位数） */
int GetMax(int *arr, int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max) max = arr[i];
    return max;
}

/*
 * 对特定位（exp=1表示个位，10表示十位...）做计数排序
 * 使用桶（0-9）收集，保证稳定性
 *
 * 时间：O(n+r)，空间：O(n+r)
 */
void CountingSortByDigit(int *arr, int n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};    /* 计数数组（基数r=10）*/

    /* 统计每个桶（0-9）的元素个数 */
    for (int i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    /* 将count变为累计计数（前缀和），表示每个桶最后一个元素的位置 */
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    /* 从后往前放置元素（保证稳定性）*/
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[--count[digit]] = arr[i];
    }

    /* 复制回原数组 */
    memcpy(arr, output, n * sizeof(int));
    free(output);
}

/*
 * LSD基数排序（从低位到高位）
 * 对每一位依次做计数排序
 *
 * ⭐ 关键：每趟必须使用稳定的内部排序
 */
void RadixSort(int *arr, int n) {
    int max = GetMax(arr, n);
    int pass = 0;

    printf("排序过程:\n");
    /* 对个位、十位、百位...依次排序 */
    for (int exp = 1; max / exp > 0; exp *= 10) {
        pass++;
        CountingSortByDigit(arr, n, exp);
        printf("第%d趟（按%s位排序）: ", pass,
               exp == 1 ? "个" : exp == 10 ? "十" : exp == 100 ? "百" : "千");
        PrintArr(arr, n);
    }
}

/* 展示桶分配过程 */
void RadixSortVerbose(int *arr, int n) {
    int max = GetMax(arr, n);

    for (int exp = 1; max / exp > 0; exp *= 10) {
        printf("按%s位分配到桶:\n", exp == 1 ? "个" : exp == 10 ? "十" : "百");
        /* 显示每个元素进哪个桶 */
        int buckets[10][100];
        int bucketSize[10] = {0};
        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10;
            buckets[digit][bucketSize[digit]++] = arr[i];
        }
        for (int d = 0; d < 10; d++) {
            if (bucketSize[d] > 0) {
                printf("  桶%d: ", d);
                for (int i = 0; i < bucketSize[d]; i++)
                    printf("%d ", buckets[d][i]);
                printf("\n");
            }
        }
        /* 收集：从桶0到桶9依次取出 */
        int idx = 0;
        for (int d = 0; d < 10; d++)
            for (int i = 0; i < bucketSize[d]; i++)
                arr[idx++] = buckets[d][i];
        printf("收集后: "); PrintArr(arr, n);
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 基数排序（LSD）演示 =====\n\n");

    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = 8;

    printf("原始数组: "); PrintArr(arr, n);
    printf("最大值=%d，需要处理3位\n\n", GetMax(arr, n));

    printf("--- 详细桶分配过程 ---\n");
    int arr2[] = {170, 45, 75, 90, 802, 24, 2, 66};
    RadixSortVerbose(arr2, n);

    printf("\n--- 基数排序汇总 ---\n");
    int arr3[] = {170, 45, 75, 90, 802, 24, 2, 66};
    RadixSort(arr3, n);
    printf("最终结果: "); PrintArr(arr3, n);

    /* 验证：考研常见3位数例子 */
    printf("\n--- 考研典型例题（3位数）---\n");
    int exam[] = {278, 109, 63, 930, 589, 184, 505, 269, 8, 83};
    int m = 10;
    printf("原始: "); PrintArr(exam, m);
    RadixSort(exam, m);

    return 0;
}
