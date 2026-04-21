/*
 * 最小生成树（MST）实现
 * 考研408数据结构 - 图
 *
 * 实现：Prim算法 和 Kruskal算法
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 10
#define MAX_E 50
#define INF 99999

/* ===== Prim算法（邻接矩阵） ===== */

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
    G->edge[v][u] = w;
}

/*
 * Prim算法
 * lowcost[v] = MST集合S中与v距离最近的边的权值
 * closest[v] = MST集合S中离v最近的顶点
 *
 * 时间复杂度：O(V²)，适合稠密图
 *
 * ⭐ 考研常考：手动推导Prim过程
 */
void Prim(Graph *G, int start) {
    int n = G->vexNum;
    int lowcost[MAX_V];     /* lowcost[v] = 连接v到MST的最短边权 */
    int closest[MAX_V];     /* closest[v] = MST中与v相连的顶点 */
    int inMST[MAX_V];       /* inMST[v] = v是否已在MST中 */

    /* 初始化：以start为起点 */
    for (int i = 0; i < n; i++) {
        lowcost[i] = G->edge[start][i];
        closest[i] = start;
        inMST[i] = 0;
    }
    inMST[start] = 1;
    int totalCost = 0;

    printf("Prim算法（起点=%d）:\n", start);

    for (int iter = 1; iter < n; iter++) {
        /* 找到不在MST中且lowcost最小的顶点 */
        int v = -1;
        for (int i = 0; i < n; i++) {
            if (!inMST[i] && (v == -1 || lowcost[i] < lowcost[v]))
                v = i;
        }
        if (v == -1 || lowcost[v] == INF) break;

        /* 将v加入MST */
        inMST[v] = 1;
        totalCost += lowcost[v];
        printf("  加入边: %d-%d (权值=%d)\n", closest[v], v, lowcost[v]);

        /* 更新其他非MST顶点的lowcost */
        for (int u = 0; u < n; u++) {
            if (!inMST[u] && G->edge[v][u] < lowcost[u]) {
                lowcost[u] = G->edge[v][u];
                closest[u] = v;
            }
        }
    }
    printf("MST总权值 = %d\n", totalCost);
}

/* ===== Kruskal算法 ===== */

/* 边结构 */
typedef struct {
    int u, v, weight;
} Edge;

/* 并查集 */
int parent[MAX_V];
int rank_uf[MAX_V];

void UFInit(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank_uf[i] = 0;
    }
}

/* 路径压缩查找 */
int UFFind(int x) {
    if (parent[x] != x)
        parent[x] = UFFind(parent[x]);  /* 路径压缩 */
    return parent[x];
}

/* 按秩合并 */
int UFUnion(int x, int y) {
    int rx = UFFind(x), ry = UFFind(y);
    if (rx == ry) return 0; /* 已在同一集合（形成环）*/
    if (rank_uf[rx] < rank_uf[ry]) { int t = rx; rx = ry; ry = t; }
    parent[ry] = rx;
    if (rank_uf[rx] == rank_uf[ry]) rank_uf[rx]++;
    return 1;
}

/* 边比较函数（用于排序） */
int CmpEdge(const void *a, const void *b) {
    return ((Edge *)a)->weight - ((Edge *)b)->weight;
}

/*
 * Kruskal算法
 * 将所有边按权值排序，依次加入不形成环的最小边
 *
 * 使用并查集判断是否形成环
 * 时间复杂度：O(E log E)，适合稀疏图
 *
 * ⭐ 考研常考：手动推导Kruskal过程
 */
void Kruskal(Edge *edges, int edgeNum, int vexNum) {
    /* 按权值排序 */
    qsort(edges, edgeNum, sizeof(Edge), CmpEdge);

    UFInit(vexNum);
    int totalCost = 0;
    int edgesAdded = 0;

    printf("Kruskal算法:\n");
    printf("排序后所有边:\n");
    for (int i = 0; i < edgeNum; i++)
        printf("  %d-%d (w=%d)\n", edges[i].u, edges[i].v, edges[i].weight);
    printf("构建MST过程:\n");

    for (int i = 0; i < edgeNum && edgesAdded < vexNum - 1; i++) {
        int u = edges[i].u, v = edges[i].v, w = edges[i].weight;
        if (UFUnion(u, v)) {    /* 不形成环，加入MST */
            printf("  加入边: %d-%d (权值=%d)\n", u, v, w);
            totalCost += w;
            edgesAdded++;
        } else {
            printf("  跳过边: %d-%d (会形成环)\n", u, v);
        }
    }
    printf("MST总权值 = %d\n", totalCost);
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 最小生成树演示 =====\n\n");
    printf("图示例:\n");
    printf("0-1(4), 0-2(2), 1-2(1), 1-3(5), 2-3(8), 2-4(10), 3-4(2), 3-5(6), 4-5(3)\n\n");

    int n = 6;

    /* Prim算法 */
    Graph G;
    InitGraph(&G, n);
    AddEdge(&G, 0, 1, 4); AddEdge(&G, 0, 2, 2); AddEdge(&G, 1, 2, 1);
    AddEdge(&G, 1, 3, 5); AddEdge(&G, 2, 3, 8); AddEdge(&G, 2, 4, 10);
    AddEdge(&G, 3, 4, 2); AddEdge(&G, 3, 5, 6); AddEdge(&G, 4, 5, 3);
    Prim(&G, 0);

    /* Kruskal算法 */
    printf("\n");
    Edge edges[] = {
        {0,1,4},{0,2,2},{1,2,1},{1,3,5},{2,3,8},
        {2,4,10},{3,4,2},{3,5,6},{4,5,3}
    };
    int edgeNum = 9;
    Kruskal(edges, edgeNum, n);

    return 0;
}
