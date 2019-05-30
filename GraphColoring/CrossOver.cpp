#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

Solution CrossOver::solve() {
    //TODO:1.首先判断population_sol中是否有冲突数为0的解，若有，直接返回此解-----大算例一般是没有
    random_select_sol();     //2.随机挑选population_sol中的cross_num个解-----将其索引保存在Set<>中，每次循环之前要clear
    SetCover setmodel(cross_num, nb_color, weight_node, graph, population_sol, cross_sol_index);    //TODO:3.定义SetModel类，更新预处理的数据结构（Model返回的是所选集合的索引值），之后处理成挑选的集合和未加的节点，作为此类的返回值
    SetOfSol sets_sol = setmodel.solve();
    //TODO:4.剩余节点加入的模型类，返回挑选的集合的数组
    //TODO:5.定义一个solution类，进行禁忌搜索，一定迭代步数内不能改进，将此解加入population_sol
    //TODO：循环，继续执行2步骤
}

void CrossOver::random_select_sol() {
    cross_sol_index.clear();
    for (int i = 0; i < cross_num; ) {
        int index = myrand.gen((int)population_sol.size() - 1);
        if (cross_sol_index.insert(index).second) { ++i; }
    }
}

//Solution CrossOver::solve() {
//    random_select_sol();          //从population_sol中随机挑两个解，保存在List中
//    prepross_sol();               //将两个解处理成两个集合列表，并记录每个冲突节点，SetSol
//    SetCoverModel cross_model(graph, set_of_crosssols, nb_color);          //用数学模型处理集合覆盖问题 (冲突节点权重--目标)
//    SetSol cross_sets = cross_model.solve();
//    //TODO：针对未在集合中的节点，用数学模型当一个小图着色处理
//    //TODO：用或者不用局部搜索处理新解，替换种群中的差解或好解，不断再用交叉算符，直到碰到合适解
//    return population_sol[cross_solu_index[0]];
//}

//void CrossOver::random_select_sol() {      //随机挑选population中的两个解
//    cross_solu_index.push_back(myrand.gen((int)population_sol.size() - 1));
//    int j = myrand.gen((int)population_sol.size() - 1);
//    while (j == cross_solu_index[0]) j = myrand.gen((int)population_sol.size() - 1);
//    cross_solu_index.push_back(j);
//    //test
//    //cross_solu_index.push_back(0);
//}
//
////TODO:当交叉多个解时需要修改
////种群中的所有解进行交叉
////void CrossOver::random_select_sol() {      
////    for (int i = 0; i < population_sol.size(); ++i)
////        cross_solu_index.push_back(i);
////}
//
//void CrossOver::prepross_sol() {
//    for (int i = 0; i < cross_solu_index.size(); ++i) {
//        List<Set<int>> sets(nb_color, Set<int>());
//        for (int j = 0; j < graph.node_num(); ++j) {
//            sets[population_sol[cross_solu_index[i]][j]].insert(j);
//        }
//        //SetSol test(sets, population_sol[cross_solu_index[i]].is_conflict_node());
//        //set_of_crosssols.push_back(test);
//        set_of_crosssols.emplace_back(sets, population_sol[cross_solu_index[i]].is_conflict_node());
//    }
//}

CrossOver::~CrossOver() {}

}
