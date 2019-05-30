#pragma once
#ifndef GRAPHCOLORING_SETCOVERMODEL_H
#define GRAPHCOLORING_SETCOVERMODEL_H

#include"InputOutput.h"

namespace graph_coloring {

//结构体：保存一个交叉解的集合和未加入的节点
//TODO:相关构造函数
struct SetOfSol {
    List<Set<int>> color_set;
    Set<int> to_add_nodes;
};

class SetCover {
public:
    SetCover(int _cross_num, int _nb_color, double _weight, const UGraph &_graph, List<Solution> &_population_sol, Set<int> &_cross_sol_index);
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
    List<Set<int>> cross_sets;         //保存颜色集合   确定
    List<int> set_parent_index;         //保存集合所在的父代索引    TODO：是记录在_population_sol中的索引，还是自己再给选出的父代重新编号？ 目前是重新编号
    HashMap<int, Set<int>> node_set_index;    //保存节点所在的集合索引   确定
    List<List<double>> is_node_conflict_parent;     //保存节点所在的父代中的权重值  TODO：数据结构是否可改,只保存有冲突的父亲节点索引，List<Set<int>>
    List<Set<int>>  parent_set_index;        //记录每个父代包含的集合的索引   不确定是否需要
};

}

//namespace graph_coloring {
//
////解的集合表示，保存解的集合和冲突节点
//struct SetSol {
//
//    SetSol(List<Set<int>> &_sets_sol, const List<bool> &_conflict_node) :
//        sets_sol(_sets_sol), conflict_node(_conflict_node) {
//    }
//    SetSol(List<Set<int>> &_sets_sol, int _nb_node) :
//        sets_sol(_sets_sol) {
//        conflict_node.resize(_nb_node, false);
//    }
//    SetSol() {
//        sets_sol.resize(0, Set<int>());
//        conflict_node.resize(0, false);
//    }
//    SetSol(SetSol &other) :sets_sol(other.sets_sol), conflict_node(other.conflict_node) {}
//    SetSol(SetSol &&other) :sets_sol(std::move(other.sets_sol)), conflict_node(std::move(other.conflict_node)) {}
//    SetSol& operator= (SetSol &rhs) {
//        sets_sol = rhs.sets_sol;
//        conflict_node = rhs.conflict_node;
//        return *this;
//    }
//    SetSol& operator=(SetSol &&rhs) {
//        sets_sol = std::move(rhs.sets_sol);
//        conflict_node = std::move(rhs.conflict_node);
//        return *this;
//    }
//
//    List<Set<int>> sets_sol;
//    List<bool> conflict_node;
//};
//
//
//class SetCoverModel {
//public:
//    SetCoverModel(const UGraph &_graph, List<SetSol> &_sets_of_sols, int _nb_color) :
//        graph(_graph), sets_of_sols(_sets_of_sols), nb_color(_nb_color) {
//        weight_of_node.reserve(graph.node_num());
//        //TODO:当交叉多个解时需要修改
//        //int cross_size = sets_of_sols.size();
//        //for (int i = 0; i < cross_size - 1; ++i) {
//        //    
//        //}
//        for (int i = 0; i < graph.node_num(); ++i) {
//            bool flag_even = sets_of_sols[0].conflict_node[i] & sets_of_sols[1].conflict_node[i];
//            bool flag_or = sets_of_sols[0].conflict_node[i] | sets_of_sols[1].conflict_node[i];
//            if (flag_even == true)weight_of_node.push_back(1);
//            else if (flag_or == true)weight_of_node.push_back(0.7);
//            else weight_of_node.push_back(0.5);
//        }
//    }
//    SetSol solve();
//private:
//    const UGraph &graph;
//    const List<SetSol> &sets_of_sols;     //交叉解的集合表示
//    int nb_color;                //要取的集合数即颜色数
//
//    List<double> weight_of_node;            //节点的权重（若在多个解中都不是冲突节点，则权重为1；权重依次降低）
//};
//
//}

#endif // ! GRAPHCOLORING_SETCOVERMODEL_H