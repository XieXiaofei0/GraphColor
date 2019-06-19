#pragma once
#ifndef GRAPHCOLORING_SETCOVERMODEL_H
#define GRAPHCOLORING_SETCOVERMODEL_H

#include"InputOutput.h"

namespace graph_coloring {

class SetCover {
public:
    SetCover(int _cross_num, int _nb_color, const UGraph &_graph, List<Solution> &_population_sol, Set<int> &_cross_sol_index, double
        _weight = 0.7, double _min_conf = 0.1, double _max_conf = 1.7, double _timeout_second = 60);
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