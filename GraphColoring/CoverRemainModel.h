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
    int nb_color;                 //��ɫ��
    int nb_to_add_nodes;          //������ڵ���

    List<int> to_add_nodes;        //������ڵ��б�
    List<List<int>> nb_conflicts_sets;    //������ڵ�����ɫ�����еĳ�ͻ��
    List<Pair<int, int>> edges_in_addnodes;   //������ڵ�֮��ı߼���

    //��ʱ����
    //List<Pair<int, int>> temp_node_color;     //����ģ�����Ľڵ�index-color��
};

}

#endif // ! GRAPHCOLORING_COVERREMAINMODEL_H