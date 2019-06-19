#pragma once
#ifndef GRAPHCOLORING_COVERREMAINMODEL_H
#define GRAPHCOLORING_COVERREMAINMODEL_H

#include"InputOutput.h"
#include"SetCoverModel.h"

namespace graph_coloring {

class CoverRemain {
public:
    CoverRemain(SetOfSol &_set_sol, const UGraph &graph);
    ~CoverRemain() {}
public:
    bool model_solve();
private:
    SetOfSol &set_sol;
    int nb_color;                 //颜色数
    int nb_to_add_nodes;          //待加入节点数

    List<int> to_add_nodes;        //待加入节点列表
    List<List<int>> nb_conflicts_sets;    //待加入节点在颜色集合中的冲突数
    List<Pair<int, int>> edges_in_addnodes;   //待加入节点之间的边集合

    //临时变量
    //List<Pair<int, int>> temp_node_color;     //保存模型求解的节点index-color对
};

}

#endif // ! GRAPHCOLORING_COVERREMAINMODEL_H