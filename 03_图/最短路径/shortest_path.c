/*
 * 最短路径算法实现
 * 考研408数据结构 - 图
 *
 * 实现：Dijkstra（单源最短路径）和 Floyd-Warshall（多源最短路径）
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_V 8
#define INF 99999

/* 邻接矩阵图 */
typedef struct {
    int edge[MAX_V][MAX_V];
    int vexNum;
} Graph;

void InitGraph(Graph *G, int n) {
    G->vexNum = n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            G->edge[i][j] = (i == j) ? 0 : INF;
}

void AddEdge(Graph *G, int u, int v, int w) {
    G->edge[u][v] = w;
    G->edge[v][u] = w;  /* 无向图 */
}

/*
 * Dijkstra算法：单源最短路径
 * 从源点 src 到所有其他顶点的最短路径
 *
 * 基于贪心：每次选取当前最短距离的未访问顶点，松弛其邻边
 * 时间复杂度：O(V²)（朴素实现）
 *
 * ⭐ 重要：不适用于负权边
 */
void Dijkstra(Graph *G, int src) {
    int dist[MAX_V];    /* dist[i] = src到i的当前最短距离 */
    bool visited[MAX_V];
    int prev[MAX_V];    /* prev[i] = 最短路径中i的前驱顶点 */
    int n = G->vexNum;

    /* 初始化 */
    for (int i = 0; i < n; i++) {
        dist[i] = G->edge[src][i];
        visited[i] = false;
        prev[i] = (dist[i] < INF) ? src : -1;
    }
    dist[src] = 0;
    visited[src] = true;

    /* 重复n-1次：每次选最小距离的未访问顶点 */
    for (int iter = 1; iter < n; iter++) {
        /* 找到dist最小的未访问顶点 u */
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }
        if (u == -1 || dist[u] == INF) break;  /* 无法到达更多顶点 */

        visited[u] = true;

        /* 松弛操作：通过u更新所有邻居的距离 */
        for (int v = 0; v < n; v++) {
            if (!visited[v] && G->edge[u][v] < INF) {
                int newDist = dist[u] + G->edge[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;    /* 更新前驱 */
                }
            }
        }
    }

    /* 输出结果 */
    printf("Dijkstra（源点=%d）:\n", src);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("  到顶点%d: 不可达\n", i);
        } else {
            /* 回溯打印路径 */
            printf("  到顶点%d: 距离=%d, 路径=", i, dist[i]);
            int path[MAX_V], pathLen = 0;
            int cur = i;
            while (cur != -1) {
                path[pathLen++] = cur;
                cur = prev[cur];
            }
            for (int j = pathLen - 1; j >= 0; j--) {
                printf("%d", path[j]);
                if (j > 0) printf("->");
            }
            printf("\n");
        }
    }
}

/*
 * Floyd-Warshall算法：多源最短路径
 * 计算所有顶点对之间的最短路径
 *
 * 动态规划：dist[i][j][k] = i到j只经过顶点{0..k}的最短路径
 * 递推：dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
 *
 * 时间复杂度：O(V³)
 * 空间复杂度：O(V²)
 */
void Floyd(Graph *G) {
    int n = G->vexNum;
    int dist[MAX_V][MAX_V];
    int path[MAX_V][MAX_V];     /* path[i][j] = i到j最短路径上j的前驱 */

    /* 初始化 */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = G->edge[i][j];
            path[i][j] = (G->edge[i][j] < INF && i != j) ? i : -1;
        }
    }

    /* 三重循环：依次以每个顶点k作为中间顶点 */
    for (int k = 0; k < n; k++) {           /* 中间顶点k */
        for (int i = 0; i < n; i++) {       /* 起点i */
            for (int j = 0; j < n; j++) {   /* 终点j */
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                        path[i][j] = path[k][j];    /* 更新路径 */
                    }
                }
            }
        }
    }

    /* 输出距离矩阵 */
    printf("Floyd 最短路径矩阵:\n    ");
    for (int i = 0; i < n; i++) printf("%5d", i);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%3d:", i);
        for (int j = 0; j < n; j++) {
            if (dist[i][j] >= INF) printf("  INF");
            else printf("%5d", dist[i][j]);
        }
        printf("\n");
    }
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 最短路径算法演示 =====\n\n");
    printf("图示例（无向带权）:\n");
    printf("0-1(4), 0-2(1), 1-3(1), 2-1(2), 2-3(5), 3-4(3)\n\n");

    Graph G;
    InitGraph(&G, 5);
    AddEdge(&G, 0, 1, 4);
    AddEdge(&G, 0, 2, 1);
    AddEdge(&G, 1, 3, 1);
    AddEdge(&G, 2, 1, 2);
    AddEdge(&G, 2, 3, 5);
    AddEdge(&G, 3, 4, 3);

    printf("--- Dijkstra算法 ---\n");
    Dijkstra(&G, 0);

    printf("\n--- Floyd算法 ---\n");
    Floyd(&G);

    return 0;
}
