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

    List<int> cross_solu_index;        //��¼Ҫ����Ľ���population_sol�е�����
    List<SetSol> set_of_crosssols;        //����������ļ���
};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
