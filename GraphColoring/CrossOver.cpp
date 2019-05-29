#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

Solution CrossOver::solve() {
    random_select_sol();          //从population_sol中随机挑两个解，保存在List中
    prepross_sol();               //将两个解处理成两个集合列表，并记录每个冲突节点，SetSol
    SetCoverModel cross_model(graph, set_of_crosssols, nb_color);          //用数学模型处理集合覆盖问题 (冲突节点权重--目标)
    SetSol cross_sets = cross_model.solve();
    //TODO：针对未在集合中的节点，用数学模型当一个小图着色处理
    //TODO：用或者不用局部搜索处理新解，替换种群中的差解或好解，不断再用交叉算符，直到碰到合适解
    return population_sol[cross_solu_index[0]];
}

void CrossOver::random_select_sol() {      //随机挑选population中的两个解
    cross_solu_index.push_back(myrand.gen((int)population_sol.size() - 1));
    int j = myrand.gen((int)population_sol.size() - 1);
    while (j == cross_solu_index[0]) j = myrand.gen((int)population_sol.size() - 1);
    cross_solu_index.push_back(j);
    //test
    //cross_solu_index.push_back(0);
}

//TODO:当交叉多个解时需要修改
//种群中的所有解进行交叉
//void CrossOver::random_select_sol() {      
//    for (int i = 0; i < population_sol.size(); ++i)
//        cross_solu_index.push_back(i);
//}

void CrossOver::prepross_sol() {
    for (int i = 0; i < cross_solu_index.size(); ++i) {
        List<Set<int>> sets(nb_color, Set<int>());
        for (int j = 0; j < graph.node_num(); ++j) {
            sets[population_sol[cross_solu_index[i]][j]].insert(j);
        }
        //SetSol test(sets, population_sol[cross_solu_index[i]].is_conflict_node());
        //set_of_crosssols.push_back(test);
        set_of_crosssols.emplace_back(sets, population_sol[cross_solu_index[i]].is_conflict_node());
    }
}

CrossOver::~CrossOver() {}

}
