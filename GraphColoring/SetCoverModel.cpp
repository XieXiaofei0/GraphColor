#include "SetCoverModel.h"

#include <sstream>

#include "utility.h"
#include "../Lib/Gurobi/include/gurobi_c++.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring{

LogSwitch logsw_modelSolver(1, 1, "ModelSolver");

String itos(int i) { stringstream s; s << i; return s.str(); }
bool equal(double lhs, double rhs, double eps = 0.00001) { return abs(lhs - rhs) < eps; }

SetSol SetCoverModel::solve() {
    int nb_parent = sets_of_sols.size();
    int nb_node = graph.node_num();
    int nb_minnum_set = nb_color / nb_parent;
    GRBEnv *env = new GRBEnv();
    GRBModel model(*env);
    //定义决策变量
    List<List<GRBVar>> select_color_parent(nb_parent, List<GRBVar>(nb_color));
    List<GRBVar> select_node(nb_node);
    for (int p = 0; p < nb_parent; ++p) {
        for (int c = 0; c < nb_color; ++c) {
            select_color_parent[p][c] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Parent_" + itos(p) + "Color_" + itos(c));
        }
    }
    for (int n = 0; n < nb_node; ++n)
        select_node[n] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "Node_" + itos(n));
    //约束
    //每个父带至少选C/P个集合--nb_minnum_set
    for (int p = 0; p < nb_parent; ++p) {
        GRBLinExpr expr;
        for (int c = 0; c < nb_color; ++c) {
            expr += select_color_parent[p][c];
        }
        model.addConstr(expr, GRB_GREATER_EQUAL, nb_minnum_set, "cons_select_min_sets_inparent_" + itos(p));
    }
    //从所有父带集合中总共选nb_color个集合
    GRBLinExpr expr_total;
    for (int p = 0; p < nb_parent; ++p) {
        for (int c = 0; c < nb_color; ++c)
            expr_total += select_color_parent[p][c];
    }
    model.addConstr(expr_total, GRB_EQUAL, nb_color, "cons_totalnum_sets");
    //顶点覆盖集合
    for (int n = 0; n < nb_node; ++n) {
        GRBLinExpr expr1;
        GRBLinExpr expr2;
        int temp = 0;
        for (int p = 0; p < nb_parent; ++p) {
            for (int c = 0; c < nb_color; ++c) {
                temp += sets_of_sols[p].sets_sol[c].count(n);
                expr1 += sets_of_sols[p].sets_sol[c].count(n)*select_node[n];
                expr2 += sets_of_sols[p].sets_sol[c].count(n)*select_color_parent[p][c];
            }
        }
        model.addConstr(expr1 - expr2, GRB_GREATER_EQUAL, 0, "cons_lower_cover_node_" + itos(n));
        model.addConstr(expr1 - expr2, GRB_LESS_EQUAL, temp - 1, "cons_upper_cover_node_" + itos(n));
    }

    //目标
    GRBLinExpr obj = 0.0;
    for (int i = 0; i < nb_node; ++i)obj += select_node[i] * weight_of_node[i];
    model.setObjective(obj, GRB_MAXIMIZE);

    mylog << "开始优化模型1" <<= logsw_modelSolver;
    model.optimize();

    int status = model.get(GRB_IntAttr_Status);
    List<Set<int>> sets_sol;
    sets_sol.reserve(nb_color);
    bool have_SetSol = false;
    if (status == GRB_OPTIMAL) {
        for (int p = 0; p < nb_parent; ++p) {
            for (int c = 0; c < nb_color; ++c) {
                if (equal(select_color_parent[p][c].get(GRB_DoubleAttr_X), 1.0)) {
                    sets_sol.emplace_back(sets_of_sols[p].sets_sol[c]);
                }
            }
        }
        have_SetSol = true;
        mylog << "获取可行解" <<= logsw_modelSolver;
    } else if (status == GRB_INFEASIBLE) {
        mylog << "无可行解" <<= logsw_modelSolver;
        model.computeIIS();
        model.write("iis.ilp");
    }
    if (have_SetSol) {
        return SetSol(sets_sol, nb_node);
    }
    return SetSol();
}

}