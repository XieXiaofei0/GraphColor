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

SetCover::SetCover(int _cross_num, int _nb_color, const double _weight, const UGraph &_graph, List<Solution> &_population_sol, Set<int> &_cross_sol_index) {
    cross_num = _cross_num;
    nb_color = _nb_color;
    nb_node = _graph.node_num();
    weight = _weight;
    cross_sets.resize(cross_num*nb_color, Set<int>());
    set_parent_index.resize(cross_num*nb_color, -1);
    is_node_conflict_parent.resize(nb_node, List<double>(cross_num, 1));
    parent_set_index.resize(cross_num, Set<int>());
    Set<int>::iterator it = _cross_sol_index.begin();
    for (int j = 0; it != _cross_sol_index.end(); ++it,++j) {          //j����ڼ�������������ѡ���ĸ�����0��ʼ���
        int parent_num = j * nb_color;
        for (int i = 0; i < nb_node; ++i) {  
            cross_sets[parent_num + _population_sol[*it][i]].insert(i);            //������ɫ����
            parent_set_index[j].insert(parent_num + _population_sol[*it][i]);       //���¸�������ɫ����index
            set_parent_index[parent_num + _population_sol[*it][i]] = j;            //������ɫ�������ڵĸ�������        TODO���ܷ�Ľ�һ��(һ�θ�ֵ֮��Ͳ��ٸ�ֵ��)
            node_set_index[i].insert(parent_num + _population_sol[*it][i]);        //���½ڵ����ڵļ���index - hashMap
            if (_population_sol[*it].is_conflict_node(i))is_node_conflict_parent[i][j] = weight;     //�����ͻ�ڵ�Ȩ��
        }
    }
}

//TODO:
SetOfSol SetCover::solve() {

}

//TODO��ת��Ϊ���ϸ��ǵ�ģ�ͣ���ѡ�еļ�������������_select_set_index
bool SetCover::setcover_model(Set<int> &_select_set_index) {
    int num_of_set = cross_sets.size();
    int num_of_node = nb_node;
    int num_of_cross = cross_num;
    GRBEnv *env = new GRBEnv();
    GRBModel model(*env);
    //model.set(GRB_DoubleParam_TimeLimit, timeout_second);    //�Ƿ����ó�ʱʱ��
    //������߱���
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
    //Լ��
    //�ڵ㸲��Լ��
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
    //����������Լ��
    //����������ѡ���ϵ�����Լ��

}

//TODO
SetOfSol SetCover::remove_dupnodes_add_nodes(const Set<int> &_select_set_index) {

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
//    //������߱���
//    List<List<GRBVar>> select_color_parent(nb_parent, List<GRBVar>(nb_color));
//    List<GRBVar> select_node(nb_node);
//    for (int p = 0; p < nb_parent; ++p) {
//        for (int c = 0; c < nb_color; ++c) {
//            select_color_parent[p][c] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Parent_" + itos(p) + "Color_" + itos(c));
//        }
//    }
//    for (int n = 0; n < nb_node; ++n)
//        select_node[n] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Node_" + itos(n));
//    //Լ��
//    //ÿ����������ѡC/P������--nb_minnum_set
//    for (int p = 0; p < nb_parent; ++p) {
//        GRBLinExpr expr;
//        for (int c = 0; c < nb_color; ++c) {
//            expr += select_color_parent[p][c];
//        }
//        model.addConstr(expr, GRB_GREATER_EQUAL, nb_minnum_set, "cons_select_min_sets_inparent_" + itos(p));
//    }
//    //�����и����������ܹ�ѡnb_color������
//    GRBLinExpr expr_total;
//    for (int p = 0; p < nb_parent; ++p) {
//        for (int c = 0; c < nb_color; ++c)
//            expr_total += select_color_parent[p][c];
//    }
//    model.addConstr(expr_total, GRB_EQUAL, nb_color, "cons_totalnum_sets");
//    //���㸲�Ǽ���
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
//    //Ŀ��
//    GRBLinExpr obj = 0.0;
//    for (int i = 0; i < nb_node; ++i)obj += select_node[i] * weight_of_node[i];
//    model.setObjective(obj, GRB_MAXIMIZE);
//
//    mylog << "��ʼ�Ż�ģ��1" <<= logsw_modelSolver;
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
//        mylog << "��ȡ���н�" <<= logsw_modelSolver;
//    } else if (status == GRB_INFEASIBLE) {
//        mylog << "�޿��н�" <<= logsw_modelSolver;
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