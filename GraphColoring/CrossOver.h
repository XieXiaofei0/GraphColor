#pragma once
#ifndef GRAPHCOLORING_CROSSOVER_H
#define GRAPHCOLORING_CROSSOVER_H
#include "InputOutput.h"
#include "utility.h"
#include "SetCoverModel.h"

namespace graph_coloring {

class CrossOver {
public:
    CrossOver(const UGraph &_graph, const int _max_population, const double _weight_node, List<Solution> &_population_sol, int _cross_num, int _nb_color) :
        graph(_graph), max_population(_max_population), weight_node(_weight_node), population_sol(_population_sol), cross_num(_cross_num), nb_color(_nb_color) {
    }
    Solution solve();
    ~CrossOver();

private:
    void random_select_sol();
    //void prepross_sol();

private:
    const UGraph &graph;
    const int max_population;
    const double weight_node;
    List<Solution> &population_sol;
    int cross_num;
    int nb_color;

    Set<int> cross_sol_index;         //��¼Ҫ����Ľ�������±�

    //List<int> cross_solu_index;        //��¼Ҫ����Ľ���population_sol�е�����
    //List<SetSol> set_of_crosssols;        //����������ļ���
};

}

#endif // !GRAPHCOLORING_CROSSOVER_H
