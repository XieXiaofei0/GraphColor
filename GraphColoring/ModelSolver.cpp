#include "ModelSolver.h"

#include <sstream>

#include "utility.h"
#include "../Lib/Gurobi/include/gurobi_c++.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

LogSwitch logsw_modelSolver(1, 1, "ModelSolver");

String itos(int i) { stringstream s; s << i; return s.str(); }
bool equal(double lhs, double rhs, double eps = 0.00001) { return abs(lhs - rhs) < eps; }

/* max: 0
* var: x_n_c = 0/1 (0: node n has not color c, 1: node n has color c)
* subject to: sum(x_n_c) == 1 for every node
*             x_i_c + x_j_c <=1 if i and j are neighbors
*/
Solution ModelSolver::solve() {
    int nb_node = graph.node_num();
    int nb_edge = graph.edge_num();
    GRBEnv *env = new GRBEnv();
    GRBModel model(*env);
    model.set(GRB_DoubleParam_TimeLimit, timeout_second); // 设置超时时间
    // 定义变量
    List<List<GRBVar>> x(nb_node, List<GRBVar>(nb_color));
    for (int n = 0; n < nb_node; ++n) {
        for (int c = 0; c < nb_color; ++c) {
            x[n][c] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + itos(n) + "_" + itos(c));
        }
    }
    // 顶点染色约束
    for (int n = 0; n < nb_node; ++n) {
        GRBLinExpr expr;
        for (int c = 0; c < nb_color; ++c) {
            expr += x[n][c];
        }
        model.addConstr(expr, GRB_EQUAL, 1.0, "A_" + itos(n));
    }
    // 邻居冲突约束
    for (int c = 0; c < nb_color; ++c) {
        for (int n = 0; n < nb_node; ++n) {
            for (int k = 0; k < graph[n].size(); ++k) {
                model.addConstr(x[n][c] + x[graph[n][k]][c], GRB_LESS_EQUAL,
                    1.0, "B_" + itos(c) + "_" + itos(n) + "_" + itos(k));
            }
        }
    }

    mylog << "开始优化模型1" <<= logsw_modelSolver;
    model.optimize();

    int status = model.get(GRB_IntAttr_Status);
    int conflict_num = nb_edge;
    List<int> node_color(nb_node, -1);
    bool have_solution = false;
    if (status == GRB_OPTIMAL) {
        for (int n = 0; n < nb_node; ++n) {
            for (int c = 0; c < nb_color; ++c) {
                if (equal(x[n][c].get(GRB_DoubleAttr_X), 1.0)) {
                    node_color[n] = c;
                }
            }
        }
        conflict_num = get_conflict_edge_num(node_color);
        have_solution = true;
        mylog << "获取可行解，冲突边数目为：" << conflict_num <<= logsw_modelSolver;
    } else if(status == GRB_TIME_LIMIT) {
        mylog << "计算超时!!!" <<= logsw_modelSolver;
    } else if (status == GRB_INFEASIBLE) {
        mylog << "无可行解" <<= logsw_modelSolver;
        model.computeIIS();
        model.write("iis.ilp");
    }
    if (have_solution) {
        return Solution(nb_color, conflict_num, node_color);
    }
    return Solution();
}

int ModelSolver::get_conflict_edge_num(List<int>& sol) {
    int count = 0;
    for (int i = 0; i < graph.edge_num(); ++i) {
        if (sol[graph.edge(i).st] == sol[graph.edge(i).ed]) {
            count++;
        }
    }
    return count;
}

}
