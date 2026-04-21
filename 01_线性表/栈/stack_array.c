/*
 * 顺序栈（Array-based Stack）实现
 * 考研408数据结构 - 栈
 *
 * 用数组实现栈，栈顶指针 top 指向栈顶元素
 * 初始 top = -1（栈空）
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 50  /* 栈的最大容量 */

/* 顺序栈定义 */
typedef struct {
    int data[MaxSize];  /* 栈的存储数组 */
    int top;            /* 栈顶指针，指向栈顶元素的下标 */
} SqStack;

/* 初始化栈 */
void InitStack(SqStack *S) {
    S->top = -1;    /* top = -1 表示栈空 */
}

/* 判断栈空 */
bool StackEmpty(SqStack *S) {
    return S->top == -1;
}

/* 判断栈满 */
bool StackFull(SqStack *S) {
    return S->top == MaxSize - 1;
}

/*
 * 入栈（Push）
 * 先将top加1，再将元素存入data[top]
 * 时间复杂度：O(1)
 */
bool Push(SqStack *S, int x) {
    if (StackFull(S)) {
        printf("栈满，无法入栈\n");
        return false;
    }
    S->data[++S->top] = x;  /* top先加1，再赋值 */
    return true;
}

/*
 * 出栈（Pop）
 * 先取出data[top]，再将top减1
 * 时间复杂度：O(1)
 */
bool Pop(SqStack *S, int *x) {
    if (StackEmpty(S)) {
        printf("栈空，无法出栈\n");
        return false;
    }
    *x = S->data[S->top--]; /* 先取值，top再减1 */
    return true;
}

/*
 * 读取栈顶元素（不出栈）
 * 时间复杂度：O(1)
 */
bool GetTop(SqStack *S, int *x) {
    if (StackEmpty(S)) {
        printf("栈空\n");
        return false;
    }
    *x = S->data[S->top];   /* 只读取，不修改top */
    return true;
}

/* 打印栈中所有元素（从栈底到栈顶） */
void PrintStack(SqStack *S) {
    printf("栈（底->顶）: [");
    for (int i = 0; i <= S->top; i++) {
        printf("%d", S->data[i]);
        if (i < S->top) printf(", ");
    }
    printf("]\n");
}

/*
 * 共享栈（两个栈共享一段连续内存）
 * 栈1从左端增长（top1从-1开始），栈2从右端增长（top2从MaxSize开始）
 * 判满条件：top1 + 1 == top2
 *
 * ⭐ 考研重点：共享栈的结构和判满条件
 */
typedef struct {
    int data[MaxSize];
    int top1;   /* 栈1的栈顶（从左往右增长） */
    int top2;   /* 栈2的栈顶（从右往左增长） */
} SharedStack;

void InitSharedStack(SharedStack *S) {
    S->top1 = -1;
    S->top2 = MaxSize;
}

bool SharedPush(SharedStack *S, int stackNum, int x) {
    /* 判满：两个栈顶相邻 */
    if (S->top1 + 1 == S->top2) {
        printf("共享栈已满\n");
        return false;
    }
    if (stackNum == 1)
        S->data[++S->top1] = x;
    else
        S->data[--S->top2] = x;
    return true;
}

bool SharedPop(SharedStack *S, int stackNum, int *x) {
    if (stackNum == 1) {
        if (S->top1 == -1) { printf("栈1为空\n"); return false; }
        *x = S->data[S->top1--];
    } else {
        if (S->top2 == MaxSize) { printf("栈2为空\n"); return false; }
        *x = S->data[S->top2++];
    }
    return true;
}

/*
 * 栈的经典应用：括号匹配
 * 遍历字符串，遇到左括号入栈，遇到右括号与栈顶匹配
 * 时间复杂度：O(n)
 *
 * ⭐ 考研高频考题
 */
bool BracketMatch(const char *str) {
    SqStack S;
    InitStack(&S);
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            Push(&S, c);    /* 左括号入栈 */
        } else if (c == ')' || c == ']' || c == '}') {
            if (StackEmpty(&S)) return false;   /* 栈空时遇到右括号，不匹配 */
            int top;
            Pop(&S, &top);
            /* 检查括号类型是否匹配 */
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
                return false;
        }
    }
    return StackEmpty(&S);  /* 最终栈空才说明完全匹配 */
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 顺序栈基本操作演示 =====\n\n");

    SqStack S;
    InitStack(&S);

    /* 入栈 */
    printf("--- 入栈操作 ---\n");
    for (int i = 1; i <= 5; i++) {
        Push(&S, i * 10);
        printf("入栈 %d -> ", i * 10);
        PrintStack(&S);
    }

    /* 读栈顶 */
    int top;
    GetTop(&S, &top);
    printf("\n栈顶元素 = %d\n", top);

    /* 出栈 */
    printf("\n--- 出栈操作 ---\n");
    int x;
    while (!StackEmpty(&S)) {
        Pop(&S, &x);
        printf("出栈 %d -> ", x);
        PrintStack(&S);
    }

    /* 共享栈演示 */
    printf("\n===== 共享栈演示 =====\n");
    SharedStack SS;
    InitSharedStack(&SS);
    SharedPush(&SS, 1, 100);
    SharedPush(&SS, 1, 200);
    SharedPush(&SS, 2, 300);
    SharedPush(&SS, 2, 400);
    printf("栈1: top1=%d, 栈2: top2=%d\n", SS.top1, SS.top2);
    SharedPop(&SS, 1, &x);
    printf("栈1出栈: %d\n", x);
    SharedPop(&SS, 2, &x);
    printf("栈2出栈: %d\n", x);

    /* 括号匹配演示 */
    printf("\n===== 括号匹配演示 =====\n");
    const char *tests[] = {"(())", "([{}])", "([)]", "((("};
    for (int i = 0; i < 4; i++) {
        printf("\"%s\" -> %s\n", tests[i],
               BracketMatch(tests[i]) ? "匹配" : "不匹配");
    }

    return 0;
}
