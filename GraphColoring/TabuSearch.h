#pragma once
#ifndef GRAPHCOLORING_TABUSEARCH_H
#define GRAPHCOLORING_TABUSEARCH_H
#include "InputOutput.h"

namespace graph_coloring {

class TabuSearch {
public:
    TabuSearch(long long max_iter, const UGraph &_graph, const Solution &_init_sol);
    Solution solve();
private:
    void update_prm_moves();
    void update_zero_moves();
    void flip_and_update_score(int node, int new_color, int old_color);
    void random_walk(int conflicting_edge, List<int> &nodeColor);
    void choose_node_color(List<int> &nodeColor);
    void init();
    void adaptNoise();
    bool choose_best_zero_move(List<int> &nodeColor);
    bool choose_best_prm_move(List<int> &nodeColor);
    int get_conflict_edge_num(List<int> &sol);
private:
    bool is_zero_move(int node, int color) {
        return node_score[node][color] == 0 && color != node_value[node];
    }
    void push_move_stack(int node, int color) {
        if (node_score[node][color] == 1) {
            PRM_MOVE_STACK.push_back(node);
            PRM_MOVE_STACK.push_back(color);
        } else if (is_zero_move(node, color)) {
            ZRO_MOVE_STACK.push_back(node);
            ZRO_MOVE_STACK.push_back(color);
        }
    }
    void push_zero_move(int node, int color) {
        if (is_zero_move(node, color)) {
            ZRO_MOVE_STACK.push_back(node);
            ZRO_MOVE_STACK.push_back(color);
        }
    }
    void push_prm_move(int node, int color) {
        if (node_score[node][color] == 1) {
            PRM_MOVE_STACK.push_back(node);
            PRM_MOVE_STACK.push_back(color);
        }
    }
private:
    const UGraph &graph;                // 拓扑图

    List<int> node_value;               // 当前解
    List<int> local_best;               // 局部最优解
    List<List<int>> node_score;         // 冲突表
    List<int> node_conflict;            // 节点冲突边数目
    List<List<long long>> flip_time;    // 节点i被染上颜色j的时刻
    List<List<long long>> leave_time;   // 节点i被去掉颜色j的时刻
    List<int> tabu;                     // 每个节点的禁忌色

    List<int> PRM_MOVE_STACK;           // 储存有希望的动作
    List<int> EDGE_STACK;               // 储存所有冲突边        
    List<int> index_in_EDGE_STACK;      // 每条边在EDGE_STACK中的索引  

    List<int> class_size;               // 每种颜色包含的顶点数目
    List<int> ZRO_MOVE_STACK;           // 储存不改变目标函数值的动作

    int NB_VALUE = -1;                  // 可用颜色数目
    int NB_NODE = -1;                   // 图中节点数目
    int NB_EDGE = -1;                   // 图中边的数目
    long long MAXSTEPS = LONG_MAX;      // 最大迭代步数

    int NOISE = 50;                     // 随机游走最大概率
    int LNOISE = 5;                     // 随机游走最小概率
    int ZNOISE = 45;                    // ZERO_MOVE发生概率
    int SNOISE = 55;                    // 

    long long cur_iter = 1;             // 当前迭代步数
    long long nbwalk = 0;               // 随机游走数目
    long long nbprm = 0;                // 有提高的动作数目
    long long nbzero = 0;               // 无变化的动作数目

    // 自适应调参相关
    long long lastAdaptFlip;
    long long lastBest;
    long long AdaptLength;
};

}

#endif // !GRAPHCOLORING_TABUSEARCH_H
