#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

Solution CrossOver::solve() {
    random_select_sol();          //从population_sol中随机挑两个解，保存在List中
    prepross_sol();               //将两个解处理成两个集合列表，并记录每个冲突节点，SetSol
    //TODO:用数学模型处理集合覆盖问题 (冲突节点权重--目标)
    //TODO：针对未在集合中的节点，用数学模型当一个小图着色处理
    //TODO：用或者不用局部搜索处理新解，替换种群中的差解或好解，不断再用交叉算符，直到碰到合适解
    return population_sol[cross_solu_index[0]];
}

void CrossOver::random_select_sol() {      //随机挑选population中的两个解
    cross_solu_index.push_back(myrand.gen((int)population_sol.size() - 1));
    int j = myrand.gen((int)population_sol.size() - 1);
    while (j == cross_solu_index[0]) j = myrand.gen((int)population_sol.size() - 1);
    cross_solu_index.push_back(j);
}

void CrossOver::prepross_sol() {
    for (int i = 0; i < cross_solu_index.size(); ++i) {
        List<Set<int>> sets(population_sol[0].nb_color());
        for (int j = 0; j < graph.node_num(); ++j) {
            sets[population_sol[cross_solu_index[i]][j]].insert(j);
        }
        set_twosols.emplace_back(sets, population_sol[cross_solu_index[i]].is_conflict_node());
    }
}

CrossOver::~CrossOver() {}

}
