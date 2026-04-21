/*
 * 图的存储结构实现
 * 考研408数据结构 - 图
 *
 * 实现邻接矩阵和邻接表两种存储方式
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 10        /* 最大顶点数 */
#define INF   99999     /* 表示无穷大（无边） */

/* ===== 邻接矩阵表示 ===== */
typedef struct {
    int vex[MAX_V];         /* 顶点表 */
    int edge[MAX_V][MAX_V]; /* 邻接矩阵：edge[i][j]=权值，INF表示无边 */
    int vexNum;             /* 顶点数 */
    int edgeNum;            /* 边数 */
} MGraph;

/* 初始化邻接矩阵图 */
void InitMGraph(MGraph *G, int n) {
    G->vexNum = n;
    G->edgeNum = 0;
    for (int i = 0; i < n; i++) {
        G->vex[i] = i;
        for (int j = 0; j < n; j++)
            G->edge[i][j] = (i == j) ? 0 : INF;
    }
}

/* 无向图：添加边 */
void AddEdge_M(MGraph *G, int u, int v, int w) {
    G->edge[u][v] = w;
    G->edge[v][u] = w;  /* 无向图对称 */
    G->edgeNum++;
}

/* 打印邻接矩阵 */
void PrintMGraph(MGraph *G) {
    printf("邻接矩阵（INF=%d表示无边）:\n    ", INF);
    for (int i = 0; i < G->vexNum; i++) printf("%5d", i);
    printf("\n");
    for (int i = 0; i < G->vexNum; i++) {
        printf("%3d:", i);
        for (int j = 0; j < G->vexNum; j++) {
            if (G->edge[i][j] == INF) printf("  INF");
            else printf("%5d", G->edge[i][j]);
        }
        printf("\n");
    }
}

/* ===== 邻接表表示 ===== */

/* 边结点 */
typedef struct ArcNode {
    int adjvex;             /* 邻接顶点编号 */
    int weight;             /* 边权值 */
    struct ArcNode *next;   /* 下一个邻接结点 */
} ArcNode;

/* 顶点结点 */
typedef struct {
    int data;               /* 顶点数据 */
    ArcNode *first;         /* 指向第一个邻接结点 */
} VNode;

/* 邻接表图 */
typedef struct {
    VNode adj[MAX_V];       /* 顶点数组 */
    int vexNum;
    int edgeNum;
} ALGraph;

/* 初始化邻接表图 */
void InitALGraph(ALGraph *G, int n) {
    G->vexNum = n;
    G->edgeNum = 0;
    for (int i = 0; i < n; i++) {
        G->adj[i].data = i;
        G->adj[i].first = NULL;
    }
}

/* 邻接表无向图添加边 */
void AddEdge_AL(ALGraph *G, int u, int v, int w) {
    /* 添加 u->v */
    ArcNode *node1 = (ArcNode *)malloc(sizeof(ArcNode));
    node1->adjvex = v;
    node1->weight = w;
    node1->next = G->adj[u].first;  /* 头插法 */
    G->adj[u].first = node1;

    /* 添加 v->u（无向图） */
    ArcNode *node2 = (ArcNode *)malloc(sizeof(ArcNode));
    node2->adjvex = u;
    node2->weight = w;
    node2->next = G->adj[v].first;
    G->adj[v].first = node2;

    G->edgeNum++;
}

/* 打印邻接表 */
void PrintALGraph(ALGraph *G) {
    printf("邻接表:\n");
    for (int i = 0; i < G->vexNum; i++) {
        printf("顶点%d:", i);
        ArcNode *p = G->adj[i].first;
        while (p) {
            printf(" -> %d(w=%d)", p->adjvex, p->weight);
            p = p->next;
        }
        printf(" -> NULL\n");
    }
}

/* 求顶点的度 */
int Degree_AL(ALGraph *G, int v) {
    int deg = 0;
    ArcNode *p = G->adj[v].first;
    while (p) { deg++; p = p->next; }
    return deg;
}

/* ===== 主函数演示 ===== */
int main(void) {
    printf("===== 图的存储结构演示 =====\n\n");
    printf("图示例（无向带权图，5个顶点）:\n");
    printf("0-1(w=4), 0-2(w=2), 1-2(w=1), 1-3(w=5), 2-4(w=3), 3-4(w=2)\n\n");

    /* 邻接矩阵 */
    printf("--- 邻接矩阵 ---\n");
    MGraph MG;
    InitMGraph(&MG, 5);
    AddEdge_M(&MG, 0, 1, 4);
    AddEdge_M(&MG, 0, 2, 2);
    AddEdge_M(&MG, 1, 2, 1);
    AddEdge_M(&MG, 1, 3, 5);
    AddEdge_M(&MG, 2, 4, 3);
    AddEdge_M(&MG, 3, 4, 2);
    PrintMGraph(&MG);

    /* 邻接表 */
    printf("\n--- 邻接表 ---\n");
    ALGraph AG;
    InitALGraph(&AG, 5);
    AddEdge_AL(&AG, 0, 1, 4);
    AddEdge_AL(&AG, 0, 2, 2);
    AddEdge_AL(&AG, 1, 2, 1);
    AddEdge_AL(&AG, 1, 3, 5);
    AddEdge_AL(&AG, 2, 4, 3);
    AddEdge_AL(&AG, 3, 4, 2);
    PrintALGraph(&AG);

    printf("\n各顶点的度:\n");
    for (int i = 0; i < AG.vexNum; i++)
        printf("顶点%d: %d\n", i, Degree_AL(&AG, i));

    return 0;
}
