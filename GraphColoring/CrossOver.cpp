#include "CrossOver.h"


using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

//Solution 
bool CrossOver::solve() {
    //TODO:1.首先判断population_sol中是否有冲突数为0的解，若有，直接返回此解-----大算例一般是没有
    random_select_sol();     //2.随机挑选population_sol中的cross_num个解-----将其索引保存在Set<>中，每次循环之前要clear
    SetCover setmodel(cross_num, nb_color, graph, population_sol, cross_sol_index);    //3.定义SetModel类，更新预处理的数据结构（模型返回的是所选集合的索引值），之后处理成挑选的集合和未加的节点，作为此类的返回值
    SetOfSol sets_sol = setmodel.solve();               //返回值为选中的集合列表和未被集合覆盖的节点列表
    CoverRemain cover_nodes(sets_sol, graph);              //TODOL:待测试 4.剩余节点加入的模型类，返回挑选的集合的数组
    bool flag = cover_nodes.model_solve();
    //TODO:5.定义一个solution类，进行禁忌搜索，一定迭代步数内不能改进，将此解加入population_sol
    //TODO：循环，继续执行2步骤
    return flag;
}

void CrossOver::random_select_sol() {
    cross_sol_index.clear();
    for (int i = 0; i < cross_num; ) {
        int index = myrand.gen((int)population_sol.size() - 1);
        if (cross_sol_index.insert(index).second) { ++i; }
    }
}

CrossOver::~CrossOver() {}

}
