#pragma once
#ifndef GRAPHCOLORING_SETCOVERMODEL_H
#define GRAPHCOLORING_SETCOVERMODEL_H

#include"InputOutput.h"

namespace graph_coloring {

//�ṹ�壺����һ�������ļ��Ϻ�δ����Ľڵ�
struct SetOfSol {
    SetOfSol() {
        color_set.resize(0, Set<int>());
        to_add_nodes.clear();
    }
    SetOfSol(List<Set<int>> &_color_set, Set<int> &_to_add_nodes) :color_set(_color_set), to_add_nodes(_to_add_nodes) {}
    SetOfSol(SetOfSol &other) :color_set(other.color_set), to_add_nodes(other.to_add_nodes) {}
    SetOfSol(SetOfSol &&other) :color_set(std::move(other.color_set)), to_add_nodes(std::move(other.to_add_nodes)) {}
    SetOfSol& operator= (SetOfSol &rhs) {
        color_set = rhs.color_set;
        to_add_nodes = rhs.to_add_nodes;
        return *this;
    }
    SetOfSol& operator=(SetOfSol &&rhs) {
        color_set = std::move(rhs.color_set);
        to_add_nodes = std::move(rhs.to_add_nodes);
        return *this;
    }
    List<Set<int>> color_set;
    Set<int> to_add_nodes;
};

class SetCover {
public:
    SetCover(int _cross_num, int _nb_color, const UGraph &_graph, List<Solution> &_population_sol, Set<int> &_cross_sol_index, double
        _weight = 0.7, double _min_conf = 0.8, double _max_conf = 1.6, double _timeout_second = 60);
    SetOfSol solve();
    ~SetCover() {}
private:
    bool setcover_model(Set<int> &_select_set_index);        //��ѧģ����⼯�ϸ��ǣ�����ѡ�еļ��ϵ�����
    SetOfSol remove_dupnodes_add_nodes(const Set<int> &_select_set_index);        //�Լ����е��ظ��ڵ�ȥ�أ�����δ����Ľڵ�
private:
    int cross_num;                      //����Ҫ����Ľ�ĸ���  ��ȷ��
    int nb_color;
    int nb_node;
    double weight;
    double at_least_sets_conf;
    double at_more_sets_conf;
    double timeout_second;
    List<Set<int>> cross_sets;         //������ɫ����   ȷ��
    List<int> set_parent_index;         //���漯�����ڵĸ�������    TODO���Ǽ�¼��_population_sol�е������������Լ��ٸ�ѡ���ĸ������±�ţ� Ŀǰ�����±��
    HashMap<int, Set<int>> node_set_index;    //����ڵ����ڵļ�������   ȷ��
    List<List<double>> is_node_conflict_parent;     //����ڵ����ڵĸ����е�Ȩ��ֵ  TODO�����ݽṹ�Ƿ�ɸ�,ֻ�����г�ͻ�ĸ��׽ڵ�������List<Set<int>>
    List<Set<int>>  parent_set_index;        //��¼ÿ�����������ļ��ϵ�����   ��ȷ���Ƿ���Ҫ
};

}

#endif // ! GRAPHCOLORING_SETCOVERMODEL_H