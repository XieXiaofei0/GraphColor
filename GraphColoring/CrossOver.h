#pragma once
#ifndef GRAPHCOLORING_CROSSOVER_H
#define GRAPHCOLORING_CROSSOVER_H
#include "InputOutput.h"
#include "utility.h"

namespace graph_coloring {

//��ļ��ϱ�ʾ�������ļ��Ϻͳ�ͻ�ڵ�
struct SetSol {    
    List<Set<int>> sets_sol;
    List<int> conflict_node;
};

class CrossOver {
public:
    CrossOver(const UGraph &_graph, List<Solution> &_population_sol) :
        graph(_graph), population_sol(_population_sol) {
    };
    Solution solve();
    ~CrossOver();

private:
    void random_select_sol();
    void prepross_sol();

private:
    const UGraph &graph;
    List<Solution> &population_sol;

    List<int> cross_solu_index;        //��¼Ҫ�������������population_sol�е�����
    List<SetSol> set_twosols;        //����������ļ���
};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
