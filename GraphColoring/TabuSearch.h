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
    const UGraph &graph;                // ����ͼ

    List<int> node_value;               // ��ǰ��
    List<int> local_best;               // �ֲ����Ž�
    List<List<int>> node_score;         // ��ͻ��
    List<int> node_conflict;            // �ڵ��ͻ����Ŀ
    List<List<long long>> flip_time;    // �ڵ�i��Ⱦ����ɫj��ʱ��
    List<List<long long>> leave_time;   // �ڵ�i��ȥ����ɫj��ʱ��
    List<int> tabu;                     // ÿ���ڵ�Ľ���ɫ

    List<int> PRM_MOVE_STACK;           // ������ϣ���Ķ���
    List<int> EDGE_STACK;               // �������г�ͻ��        
    List<int> index_in_EDGE_STACK;      // ÿ������EDGE_STACK�е�����  

    List<int> class_size;               // ÿ����ɫ�����Ķ�����Ŀ
    List<int> ZRO_MOVE_STACK;           // ���治�ı�Ŀ�꺯��ֵ�Ķ���

    int NB_VALUE = -1;                  // ������ɫ��Ŀ
    int NB_NODE = -1;                   // ͼ�нڵ���Ŀ
    int NB_EDGE = -1;                   // ͼ�бߵ���Ŀ
    long long MAXSTEPS = LONG_MAX;      // ����������

    int NOISE = 50;                     // �������������
    int LNOISE = 5;                     // ���������С����
    int ZNOISE = 45;                    // ZERO_MOVE��������
    int SNOISE = 55;                    // 

    long long cur_iter = 1;             // ��ǰ��������
    long long nbwalk = 0;               // ���������Ŀ
    long long nbprm = 0;                // ����ߵĶ�����Ŀ
    long long nbzero = 0;               // �ޱ仯�Ķ�����Ŀ

    // ����Ӧ�������
    long long lastAdaptFlip;
    long long lastBest;
    long long AdaptLength;
};

}

#endif // !GRAPHCOLORING_TABUSEARCH_H
