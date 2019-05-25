#pragma once
#ifndef GRAPHCOLORING_CROSSOVER_H
#define GRAPHCOLORING_CROSSOVER_H
#include "InputOutput.h"
#include "utility.h"

namespace graph_coloring {

//解的集合表示，保存解的集合和冲突节点
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

    List<int> cross_solu_index;        //记录要交叉的两个解在population_sol中的索引
    List<SetSol> set_twosols;        //保存两个解的集合
};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
