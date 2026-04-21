/*
 * 图的遍历：DFS 和 BFS
 * 考研408数据结构 - 图
 *
 * 基于邻接表实现DFS和BFS遍历
 * 时间复杂度（邻接表）：O(V+E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_V 10

/* 边结点 */
typedef struct ArcNode {
    int adjvex;
    struct ArcNode *next;
} ArcNode;

/* 顶点结点 */
typedef struct {
    int data;
    ArcNode *first;
} VNode;

/* 邻接表图 */
typedef struct {
    VNode adj[MAX_V];
    int vexNum, edgeNum;
} Graph;

/* 初始化图 */
void InitGraph(Graph *G, int n) {
    G->vexNum = n; G->edgeNum = 0;
    for (int i = 0; i < n; i++) {
        G->adj[i].data = i;
        G->adj[i].first = NULL;
    }
}

/* 添加无向边 */
void AddEdge(Graph *G, int u, int v) {
    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = v; p->next = G->adj[u].first; G->adj[u].first = p;
    ArcNode *q = (ArcNode *)malloc(sizeof(ArcNode));
    q->adjvex = u; q->next = G->adj[v].first; G->adj[v].first = q;
    G->edgeNum++;
}

/* 访问标记数组 */
bool visited[MAX_V];

/*
 * DFS：深度优先搜索（递归）
 * 类似树的先序遍历
 * 时间复杂度：O(V+E)（邻接表）
 */
void DFS(Graph *G, int v) {
    visited[v] = true;
    printf("%d ", v);   /* 访问顶点v */
    ArcNode *p = G->adj[v].first;
    while (p != NULL) {
        if (!visited[p->adjvex])
            DFS(G, p->adjvex);  /* 递归访问未访问的邻居 */
        p = p->next;
    }
}

/*
 * DFS遍历整个图（处理非连通图）
 * 对每个未访问的顶点调用DFS
 */
void DFS_Traverse(Graph *G) {
    memset(visited, false, sizeof(visited));
    printf("DFS遍历: ");
    for (int i = 0; i < G->vexNum; i++) {
        if (!visited[i])
            DFS(G, i);
    }
    printf("\n");
}

/*
 * BFS：广度优先搜索
 * 类似树的层序遍历，使用队列
 * 时间复杂度：O(V+E)（邻接表）
 *
 * ⭐ BFS可以求无权图的最短路径
 */
void BFS(Graph *G, int start) {
    int queue[MAX_V];
    int front = 0, rear = 0;

    visited[start] = true;
    queue[rear++] = start;

    while (front != rear) {
        int v = queue[front++];
        printf("%d ", v);   /* 访问顶点v */

        ArcNode *p = G->adj[v].first;
        while (p != NULL) {
            if (!visited[p->adjvex]) {
                visited[p->adjvex] = true;
                queue[rear++] = p->adjvex;  /* 未访问的邻居入队 */
            }
            p = p->next;
        }
    }
}

/* BFS遍历整个图 */
void BFS_Traverse(Graph *G) {
    memset(visited, false, sizeof(visited));
    printf("BFS遍历: ");
    for (int i = 0; i < G->vexNum; i++) {
        if (!visited[i])
            BFS(G, i);
    }
    printf("\n");
}

/*
 * BFS求无权图最短路径（以跳数为距离）
 * dist[i] 存储从start到顶点i的最短跳数
 */
void BFS_ShortestPath(Graph *G, int start) {
    int dist[MAX_V];
    int prev[MAX_V];    /* 记录路径的前驱结点 */
    for (int i = 0; i < G->vexNum; i++) {
        dist[i] = -1;   /* -1表示未访问 */
        prev[i] = -1;
    }

    int queue[MAX_V];
    int front = 0, rear = 0;

    dist[start] = 0;
    queue[rear++] = start;

    while (front != rear) {
        int v = queue[front++];
        ArcNode *p = G->adj[v].first;
        while (p != NULL) {
            if (dist[p->adjvex] == -1) {
                dist[p->adjvex] = dist[v] + 1;
                prev[p->adjvex] = v;
                queue[rear++] = p->adjvex;
            }
            p = p->next;
        }
    }

    printf("从顶点%d出发的BFS最短路径（无权图）:\n", start);
    for (int i = 0; i < G->vexNum; i++) {
        printf("  到顶点%d: 距离=%d\n", i, dist[i]);
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 图的遍历演示 =====\n\n");
    printf("图示例:\n");
    printf("0-1, 0-2, 1-3, 1-4, 2-4, 3-5, 4-5\n\n");

    Graph G;
    InitGraph(&G, 6);
    AddEdge(&G, 0, 1);
    AddEdge(&G, 0, 2);
    AddEdge(&G, 1, 3);
    AddEdge(&G, 1, 4);
    AddEdge(&G, 2, 4);
    AddEdge(&G, 3, 5);
    AddEdge(&G, 4, 5);

    DFS_Traverse(&G);
    BFS_Traverse(&G);

    printf("\n");
    BFS_ShortestPath(&G, 0);

    return 0;
}
