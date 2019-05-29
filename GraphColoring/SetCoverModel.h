#pragma once
#ifndef GRAPHCOLORING_SETCOVERMODEL_H
#define GRAPHCOLORING_SETCOVERMODEL_H

#include"InputOutput.h"

namespace graph_coloring {

//��ļ��ϱ�ʾ�������ļ��Ϻͳ�ͻ�ڵ�
struct SetSol {

    SetSol(List<Set<int>> &_sets_sol, const List<bool> &_conflict_node) :
        sets_sol(_sets_sol), conflict_node(_conflict_node) {
    }
    SetSol(List<Set<int>> &_sets_sol, int _nb_node) :
        sets_sol(_sets_sol) {
        conflict_node.resize(_nb_node, false);
    }
    SetSol() {
        sets_sol.resize(0, Set<int>());
        conflict_node.resize(0, false);
    }
    SetSol(SetSol &other) :sets_sol(other.sets_sol), conflict_node(other.conflict_node) {}
    SetSol(SetSol &&other) :sets_sol(std::move(other.sets_sol)), conflict_node(std::move(other.conflict_node)) {}
    SetSol& operator= (SetSol &rhs) {
        sets_sol = rhs.sets_sol;
        conflict_node = rhs.conflict_node;
        return *this;
    }
    SetSol& operator=(SetSol &&rhs) {
        sets_sol = std::move(rhs.sets_sol);
        conflict_node = std::move(rhs.conflict_node);
        return *this;
    }

    List<Set<int>> sets_sol;
    List<bool> conflict_node;
};


class SetCoverModel {
public:
    SetCoverModel(const UGraph &_graph, List<SetSol> &_sets_of_sols, int _nb_color) :
        graph(_graph), sets_of_sols(_sets_of_sols), nb_color(_nb_color) {
        weight_of_node.reserve(graph.node_num());
        //TODO:����������ʱ��Ҫ�޸�
        //int cross_size = sets_of_sols.size();
        //for (int i = 0; i < cross_size - 1; ++i) {
        //    
        //}
        for (int i = 0; i < graph.node_num(); ++i) {
            bool flag_even = sets_of_sols[0].conflict_node[i] & sets_of_sols[1].conflict_node[i];
            bool flag_or = sets_of_sols[0].conflict_node[i] | sets_of_sols[1].conflict_node[i];
            if (flag_even == true)weight_of_node.push_back(1);
            else if (flag_or == true)weight_of_node.push_back(0.7);
            else weight_of_node.push_back(0.5);
        }
    }
    SetSol solve();
private:
    const UGraph &graph;
    const List<SetSol> &sets_of_sols;     //�����ļ��ϱ�ʾ
    int nb_color;                //Ҫȡ�ļ���������ɫ��

    List<double> weight_of_node;            //�ڵ��Ȩ�أ����ڶ�����ж����ǳ�ͻ�ڵ㣬��Ȩ��Ϊ1��Ȩ�����ν��ͣ�
};

}

#endif // ! GRAPHCOLORING_SETCOVERMODEL_H