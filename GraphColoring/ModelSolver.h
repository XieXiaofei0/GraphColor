#pragma once
#ifndef GRAPHCOLORING_MODELSOLVER_H
#define GRAPHCOLORING_MODELSOLVER_H
#include "InputOutput.h"

namespace graph_coloring {

class ModelSolver {
public:
    ModelSolver(UGraph &_graph, int _nb_color, double _timeout_second = 60.0):
        graph(_graph), nb_color(_nb_color), timeout_second(_timeout_second) {}
    Solution solve();
private:
    int get_conflict_edge_num(List<int> &sol);
private:
    UGraph &graph;
    int nb_color;
    double timeout_second;
};

}

#endif // ! GRAPHCOLORING_MODELSOLVER_H
