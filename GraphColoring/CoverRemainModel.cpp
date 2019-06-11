#include"CoverRemainModel.h"

#include"utility.h"
#include"../Lib/Gurobi/include/gurobi_c++.h"

using namespace std;
using namespace zjl_utility;

namespace  graph_coloring {

LogSwitch logsw_modelSolver2(1, 1, "Modle2Solver");

String itos(int i);
bool equal(double lhs, double rhs, double eps = 0.00001);

CoverRemain::CoverRemain(SetOfSol &_set_sol, const UGraph &graph):set_sol(_set_sol){
    nb_color = set_sol.color_set.size();
    nb_to_add_nodes = set_sol.to_add_nodes.size();
    to_add_nodes.reserve(nb_to_add_nodes);
    nb_conflicts_sets.resize(nb_to_add_nodes, List<int>(nb_color, 0));
    edges_in_addnodes.reserve(nb_to_add_nodes*(nb_to_add_nodes - 1) / 2);
    HashMap<int, int> node_index;               //临时变量，记录节点在数组to_add_nodes中的索引值
    int index = 0;
    for (auto node : set_sol.to_add_nodes) {
        to_add_nodes.push_back(node);
        node_index[node] = index++;
    }
    for (int n = 0; n < nb_to_add_nodes; ++n) {
        Set<int> adj_nodes;    //保存节点n的相邻节点集合
        for (auto adj_node : graph[to_add_nodes[n]]) adj_nodes.insert(adj_node);
        List<int> temp_repeat(nb_to_add_nodes);      //保存重复节点
        //List<int>::iterator it;
        auto it = set_intersection(adj_nodes.begin(), adj_nodes.end(), set_sol.to_add_nodes.begin(), set_sol.to_add_nodes.end(), temp_repeat.begin());
        temp_repeat.resize(it - temp_repeat.begin());
        for (int i = 0; i < temp_repeat.size(); ++i) { 
            if (n < node_index[temp_repeat[i]]) {             //去掉重复边
                edges_in_addnodes.push_back(make_pair(n, node_index[temp_repeat[i]]));
            }
        }
        for (int i = 0; i < set_sol.color_set.size(); ++i) {
            List<int> temp(set_sol.color_set[i].size());
            //List<int>::iterator it;
            auto it = set_intersection(adj_nodes.begin(), adj_nodes.end(), set_sol.color_set[i].begin(), set_sol.color_set[i].end(), temp.begin());
            temp.resize(it - temp_repeat.begin());
            nb_conflicts_sets[n][i] = temp.size();
        }
    }
}

//TODO:将剩余节点加入set_sol的cross_set中
bool CoverRemain::model_solve() {
    int num_of_color = nb_color;
    int num_of_renodes = nb_to_add_nodes;
    GRBEnv *env = new GRBEnv();
    GRBModel model(*env);
    //定义决策变量
    List<List<GRBVar>> x_node_color(num_of_renodes, List<GRBVar>(num_of_color));
    List<GRBVar> y_edge(edges_in_addnodes.size());
    for (int n = 0; n < num_of_renodes; ++n) {
        for (int c = 0; c < num_of_color; ++c) {
            x_node_color[n][c] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_node_" + itos(n) + "_color_" + itos(c));
        }
    }
    for (int i = 0; i < edges_in_addnodes.size(); ++i) {
        y_edge[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y_edge_" + itos(i));
    }
    //约束
    for (int n = 0; n < num_of_renodes; ++n) {
        GRBLinExpr expr;
        for (int c = 0; c < num_of_color; ++c) {
            expr += x_node_color[n][c];
        }
        model.addConstr(expr, GRB_EQUAL, 1, "Cons_x_node_" + itos(n));
    }
    for (int c = 0; c < num_of_color; ++c) {
        for (int i = 0; i < edges_in_addnodes.size(); ++i) {
            GRBLinExpr expr;
            expr += x_node_color[edges_in_addnodes[i].first][c];
            expr += x_node_color[edges_in_addnodes[i].second][c];
            expr -= 2 * y_edge[i];
            model.addConstr(expr, GRB_LESS_EQUAL, 1, "Cons_y_max_edge_" + itos(i));
            model.addConstr(expr, GRB_GREATER_EQUAL, 0, "Cons_y_min_edge_" + itos(i));
        }
    }
    //目标
    GRBLinExpr obj = 0.0;
    for (int n = 0; n < num_of_renodes; ++n) {
        for (int c = 0; c < num_of_color; ++c) {
            obj += x_node_color[n][c] * nb_conflicts_sets[n][c];
        }
    }
    for (int i = 0; i < edges_in_addnodes.size(); ++i)obj += y_edge[i];
    model.setObjective(obj, GRB_MINIMIZE);

    mylog << "开始优化模型1" <<= logsw_modelSolver2;
    model.optimize();

    int status = model.get(GRB_IntAttr_Status);
    bool have_solve = false;
    if (status == GRB_OPTIMAL) {
        temp_node_color.reserve(num_of_renodes);
        for (int n = 0; n < num_of_renodes; ++n) {
            for (int c = 0; c < num_of_color; ++c) {
                if (equal(x_node_color[n][c].get(GRB_DoubleAttr_X), 1.0)) {
                    //TODO:将剩余节点加入set_sol的cross_set中
                    temp_node_color.push_back(make_pair(to_add_nodes[n], c));
                }
            }
        }
        have_solve = true;
        mylog << "获取可行解" <<= logsw_modelSolver2;
    }
    else if (status == GRB_INFEASIBLE) {
        mylog << "无可行解" <<= logsw_modelSolver2;
        model.computeIIS();
        model.write("iis.ilp");
    }
    return have_solve;
}

}