#pragma once
#ifndef GRAPHCOLORING_CROSSOVER_H
#define GRAPHCOLORING_CROSSOVER_H
#include "InputOutput.h"
#include "utility.h"
#include "SetCoverModel.h"

namespace graph_coloring {

class CrossOver {
public:
    CrossOver(const UGraph &_graph, const int _max_population, List<Solution> &_population_sol, int _cross_num, int _nb_color) :
        graph(_graph), max_population(_max_population), population_sol(_population_sol), cross_num(_cross_num), nb_color(_nb_color) {
    }
    //Solution solve();
    bool solve();
    ~CrossOver();

private:
    void random_select_sol();

private:
    const UGraph &graph;
    const int max_population;
    List<Solution> &population_sol;
    int cross_num;
    int nb_color;

    Set<int> cross_sol_index;         //记录要交叉的解的索引下标

};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
