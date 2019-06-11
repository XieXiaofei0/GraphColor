#pragma once
#ifndef GRAPHCOLORING_SETCOVERMODEL_H
#define GRAPHCOLORING_SETCOVERMODEL_H

#include"InputOutput.h"

namespace graph_coloring {

//结构体：保存一个交叉解的集合和未加入的节点
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
    bool setcover_model(Set<int> &_select_set_index);        //数学模型求解集合覆盖，保存选中的集合的索引
    SetOfSol remove_dupnodes_add_nodes(const Set<int> &_select_set_index);        //对集合中的重复节点去重，保存未加入的节点
private:
    int cross_num;                      //保存要交叉的解的个数  不确定
    int nb_color;
    int nb_node;
    double weight;
    double at_least_sets_conf;
    double at_more_sets_conf;
    double timeout_second;
    List<Set<int>> cross_sets;         //保存颜色集合   确定
    List<int> set_parent_index;         //保存集合所在的父代索引    TODO：是记录在_population_sol中的索引，还是自己再给选出的父代重新编号？ 目前是重新编号
    HashMap<int, Set<int>> node_set_index;    //保存节点所在的集合索引   确定
    List<List<double>> is_node_conflict_parent;     //保存节点所在的父代中的权重值  TODO：数据结构是否可改,只保存有冲突的父亲节点索引，List<Set<int>>
    List<Set<int>>  parent_set_index;        //记录每个父代包含的集合的索引   不确定是否需要
};

}

#endif // ! GRAPHCOLORING_SETCOVERMODEL_H