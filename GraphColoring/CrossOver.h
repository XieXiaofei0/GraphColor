#pragma once
#ifndef GRAPHCOLORING_CROSSOVER_H
#define GRAPHCOLORING_CROSSOVER_H
#include "InputOutput.h"
#include "utility.h"
#include "SetCoverModel.h"

namespace graph_coloring {

class CrossOver {
public:
    CrossOver(const UGraph &_graph, List<Solution> &_population_sol, int _nb_color) :
        graph(_graph), population_sol(_population_sol),nb_color(_nb_color){
    };
    Solution solve();
    ~CrossOver();

private:
    void random_select_sol();
    void prepross_sol();

private:
    const UGraph &graph;
    List<Solution> &population_sol;
    int nb_color;

    List<int> cross_solu_index;        //记录要交叉的解在population_sol中的索引
    List<SetSol> set_of_crosssols;        //保存两个解的集合
};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
