#include "SetCoverModel.h"

#include <sstream>

#include "utility.h"
#include "../Lib/Gurobi/include/gurobi_c++.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

LogSwitch logsw_modelSolver(1, 1, "ModelSolver");

String itos(int i) { stringstream s; s << i; return s.str(); }
bool equal(double lhs, double rhs, double eps = 0.00001) { return abs(lhs - rhs) < eps; }

SetCover::SetCover(int _cross_num, int _nb_color, const UGraph &_graph, List<Solution> &_population_sol, Set<int> &_cross_sol_index, double
    _weight , double _min_conf , double _max_conf , double _timeout_second ) {
    cross_num = _cross_num;
    nb_color = _nb_color;
    nb_node = _graph.node_num();
    weight = _weight;
    at_least_sets_conf = _min_conf;
    at_more_sets_conf = _max_conf;
    timeout_second = _timeout_second;
    cross_sets.resize(cross_num*nb_color, Set<int>());
    set_parent_index.resize(cross_num*nb_color, -1);
    is_node_conflict_parent.resize(nb_node, List<double>(cross_num, 1));
    parent_set_index.resize(cross_num, Set<int>());
    Set<int>::iterator it = _cross_sol_index.begin();
    for (int j = 0; it != _cross_sol_index.end(); ++it,++j) {          //j代表第几个父代，将挑选出的父代从0开始编号
        int parent_num = j * nb_color;
        for (int i = 0; i < nb_node; ++i) {  
            cross_sets[parent_num + _population_sol[*it][i]].insert(i);            //更新颜色集合
            parent_set_index[j].insert(parent_num + _population_sol[*it][i]);       //更新父代的颜色集合index
            set_parent_index[parent_num + _population_sol[*it][i]] = j;            //更新颜色集合所在的父代索引        TODO：能否改进一下(一次赋值之后就不再赋值了)
            node_set_index[i].insert(parent_num + _population_sol[*it][i]);        //更新节点所在的集合index - hashMap
            if (_population_sol[*it].is_conflict_node(i))is_node_conflict_parent[i][j] = weight;     //保存冲突节点权重
        }
    }
}

//集合覆盖返回
SetOfSol SetCover::solve() {
    Set<int> select_set_index;
    select_set_index.clear();
    if (setcover_model(select_set_index)) {
        return remove_dupnodes_add_nodes(select_set_index);
    }
    else {
        mylog << "集合覆盖模型无可行解" <<= logsw_info;
        return SetOfSol();
    }
}

//转换为集合覆盖的模型，将选中的集合索引保存在_select_set_index
bool SetCover::setcover_model(Set<int> &_select_set_index) {
    int num_of_set = cross_sets.size();
    int num_of_node = nb_node;
    int num_of_cross = cross_num;
    GRBEnv *env = new GRBEnv();
    GRBModel model(*env);
    //model.set(GRB_DoubleParam_TimeLimit, timeout_second);    //是否设置超时时间
    //定义决策变量
    List<GRBVar> s_set(num_of_set);
    List<GRBVar> l_node(num_of_node);
    List<List<GRBVar>> aux_y(num_of_node, List<GRBVar>(num_of_set));
    List<GRBVar> aux_z(num_of_node);
    for (int s = 0; s < num_of_set; ++s) {
        s_set[s] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "s_set_" + itos(s));
    }
    for (int n = 0; n < num_of_node; ++n) {
        l_node[n] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "s_node_" + itos(n));
        aux_z[n] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "aux_z_" + itos(n));
        Set<int>::iterator it = node_set_index[n].begin();
        for (; it != node_set_index[n].end(); ++it) {
            aux_y[n][*it] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "aux_y_" + itos(n) + "_" + itos(*it));
        }
    }
    //约束
    //节点覆盖约束
    for (int n = 0; n < num_of_node; ++n) {
        model.addConstr(aux_z[n], GRB_GREATER_EQUAL, l_node[n], "aux_z_gE_node" + itos(n));
        GRBLinExpr expr2;
        Set<int>::iterator it = node_set_index[n].begin();
        for (; it != node_set_index[n].end(); ++it) {
            GRBLinExpr expr1;
            expr1 += aux_y[n][*it];
            expr1 += is_node_conflict_parent[n][set_parent_index[*it]] * s_set[*it];
            model.addConstr(expr1, GRB_GREATER_EQUAL, aux_z[n], "aux_y_" + itos(*it) + "_" + itos(n));
            expr2 += aux_y[n][*it];
        }
        model.addConstr(expr2, GRB_LESS_EQUAL, node_set_index[n].size() - 1, "aux_y_" + itos(n));
    }
    //集合总数量约束 >= C
    GRBLinExpr expr3;
    for (int s = 0; s < num_of_set; ++s) {
        expr3 += s_set[s];
    }
    model.addConstr(expr3, GRB_EQUAL, nb_color, "num_of_set_" + itos(nb_color));
    //单个父代所选集合的数量约束
    int min = at_least_sets_conf * (nb_color / cross_num);
    int max = at_more_sets_conf * (nb_color / cross_num);
    for (int i = 0; i < parent_set_index.size(); ++i) {
        GRBLinExpr expr;
        Set<int>::iterator it = parent_set_index[i].begin();
        for (; it != parent_set_index[i].end(); ++it) {
            expr += s_set[*it];
        }
        model.addConstr(expr, GRB_GREATER_EQUAL, min, "parent_" + itos(i) + "_min");
        model.addConstr(expr, GRB_LESS_EQUAL, max, "parent_" + itos(i) + "_max");
    }
    //目标:
    GRBLinExpr obj = 0.0;
    for (int n = 0; n < num_of_node; ++n)obj += l_node[n];
    model.setObjective(obj, GRB_MAXIMIZE);

    mylog << "开始优化模型1" <<= logsw_modelSolver;
    model.optimize();

    int status = model.get(GRB_IntAttr_Status);
    bool have_select = false;
    if (status == GRB_OPTIMAL) {
        for (int s = 0; s < num_of_set; ++s) {
            if (equal(s_set[s].get(GRB_DoubleAttr_X), 1.0))_select_set_index.insert(s);
        }
        have_select = true;
        mylog << "获取可行解" <<= logsw_modelSolver;
    } else if (status == GRB_INFEASIBLE) {
        mylog << "无可行解" <<= logsw_modelSolver;
        model.computeIIS();
        model.write("iis.ilp");
    }
    return have_select;
}

//TODO
SetOfSol SetCover::remove_dupnodes_add_nodes(const Set<int> &_select_set_index) {
    List<Set<int>> sol_set(nb_color, Set<int>());
    Set<int> add_nodes;
    for (int n = 0; n < nb_node; ++n) {
        vector<int> repeat_node(cross_num);    //记录重复节点所在的集合索引
        vector<int>::iterator it;
        it = set_intersection(_select_set_index.begin(), _select_set_index.end(), node_set_index[n].begin(), node_set_index[n].end(), it);
        repeat_node.resize(it - repeat_node.begin());
        if (repeat_node.size() == 0) {    //未加入的节点
            add_nodes.insert(n);
        } else if (repeat_node.size() > 1) {   //重复出现的节点

        }
    }
}

}

//namespace graph_coloring{
//
//LogSwitch logsw_modelSolver(1, 1, "ModelSolver");
//
//String itos(int i) { stringstream s; s << i; return s.str(); }
//bool equal(double lhs, double rhs, double eps = 0.00001) { return abs(lhs - rhs) < eps; }
//
//SetSol SetCoverModel::solve() {
//    int nb_parent = sets_of_sols.size();
//    int nb_node = graph.node_num();
//    int nb_minnum_set = nb_color / nb_parent;
//    GRBEnv *env = new GRBEnv();
//    GRBModel model(*env);
//    //定义决策变量
//    List<List<GRBVar>> select_color_parent(nb_parent, List<GRBVar>(nb_color));
//    List<GRBVar> select_node(nb_node);
//    for (int p = 0; p < nb_parent; ++p) {
//        for (int c = 0; c < nb_color; ++c) {
//            select_color_parent[p][c] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Parent_" + itos(p) + "Color_" + itos(c));
//        }
//    }
//    for (int n = 0; n < nb_node; ++n)
//        select_node[n] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Node_" + itos(n));
//    //约束
//    //每个父带至少选C/P个集合--nb_minnum_set
//    for (int p = 0; p < nb_parent; ++p) {
//        GRBLinExpr expr;
//        for (int c = 0; c < nb_color; ++c) {
//            expr += select_color_parent[p][c];
//        }
//        model.addConstr(expr, GRB_GREATER_EQUAL, nb_minnum_set, "cons_select_min_sets_inparent_" + itos(p));
//    }
//    //从所有父带集合中总共选nb_color个集合
//    GRBLinExpr expr_total;
//    for (int p = 0; p < nb_parent; ++p) {
//        for (int c = 0; c < nb_color; ++c)
//            expr_total += select_color_parent[p][c];
//    }
//    model.addConstr(expr_total, GRB_EQUAL, nb_color, "cons_totalnum_sets");
//    //顶点覆盖集合
//    for (int n = 0; n < nb_node; ++n) {
//        GRBLinExpr expr1;
//        GRBLinExpr expr2;
//        int temp = 0;
//        for (int p = 0; p < nb_parent; ++p) {
//            for (int c = 0; c < nb_color; ++c) {
//                temp += sets_of_sols[p].sets_sol[c].count(n);
//                expr1 += sets_of_sols[p].sets_sol[c].count(n)*select_node[n];
//                expr2 += sets_of_sols[p].sets_sol[c].count(n)*select_color_parent[p][c];
//            }
//        }
//        model.addConstr(expr1 - expr2, GRB_GREATER_EQUAL, 0, "cons_lower_cover_node_" + itos(n));
//        model.addConstr(expr1 - expr2, GRB_LESS_EQUAL, temp - 1, "cons_upper_cover_node_" + itos(n));
//    }
//
//    //目标
//    GRBLinExpr obj = 0.0;
//    for (int i = 0; i < nb_node; ++i)obj += select_node[i] * weight_of_node[i];
//    model.setObjective(obj, GRB_MAXIMIZE);
//
//    mylog << "开始优化模型1" <<= logsw_modelSolver;
//    model.optimize();
//
//    int status = model.get(GRB_IntAttr_Status);
//    List<Set<int>> sets_sol;
//    sets_sol.reserve(nb_color);
//    bool have_SetSol = false;
//    if (status == GRB_OPTIMAL) {
//        for (int p = 0; p < nb_parent; ++p) {
//            for (int c = 0; c < nb_color; ++c) {
//                if (equal(select_color_parent[p][c].get(GRB_DoubleAttr_X), 1.0)) {
//                    sets_sol.emplace_back(sets_of_sols[p].sets_sol[c]);
//                }
//            }
//        }
//        have_SetSol = true;
//        mylog << "获取可行解" <<= logsw_modelSolver;
//    } else if (status == GRB_INFEASIBLE) {
//        mylog << "无可行解" <<= logsw_modelSolver;
//        model.computeIIS();
//        model.write("iis.ilp");
//    }
//    if (have_SetSol) {
//        return SetSol(sets_sol, nb_node);
//    }
//    return SetSol();
//}
//
//}