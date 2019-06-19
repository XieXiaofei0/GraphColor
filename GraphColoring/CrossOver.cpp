#include "CrossOver.h"


using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

//Solution 
bool CrossOver::solve() {
    //方法一：直接两个解进行交叉
    //random_select_sol();
    cross_sol_index.clear();
    cross_sol_index.insert(0);
    cross_sol_index.insert(1);

    //test
    mylog << "选出的交叉解的冲突数：  " << population_sol[0].nb_conflict() << "     " << population_sol[1].nb_conflict() <<= logsw_info;
    //cout << endl;
    //cout << "选出的交叉解的冲突数：" << endl;
    //for (auto i : cross_sol_index) {
    //     cout << "             " << i << "     冲突数：" << population_sol[i].nb_conflict() << endl;
    //}
    //cout << endl << endl;
    //test end

    SetCover setmodel(cross_num, nb_color, graph, population_sol, cross_sol_index);    
    SetOfSol sets_sol = setmodel.solve();               
    CoverRemain cover_nodes(sets_sol, graph);              
    bool flag = cover_nodes.model_solve();
    if (flag) {                                
        Solution cross_sol(graph.node_num(), nb_color);
        cross_sol.setInit(sets_sol);
        TabuSearch tabu(3000 * 1000, graph, cross_sol);

        //tet
        mylog << "交叉之后的冲突边数目为：" << tabu.num_of_conflict() <<= logsw_info;
        //test end

        Solution new_sol = tabu.solve();
        if (new_sol.nb_conflict() == 0)return true;
            
        //test
        mylog << "交叉解禁忌搜索后的冲突边数目为：" << new_sol.nb_conflict() <<= logsw_info;
        //test end
    }
    return false;
    //方法二：不断添加更新population中的解
    //TODO:1.首先判断population_sol中是否有冲突数为0的解，若有，直接返回此解-----大算例一般是没有
    //while (population_sol.size() < 100) {
    //    random_select_sol();     //2.随机挑选population_sol中的cross_num个解-----将其索引保存在Set<>中，每次循环之前要clear

    //    //test:         TODO:随机挑选的交叉解的冲突数
    //    cout << endl;
    //    cout << "选出的交叉解的冲突数：" << endl;
    //    for (auto i : cross_sol_index) {
    //        cout << "             " << i << "     冲突数：" << population_sol[i].nb_conflict() << endl;
    //    }
    //    cout << endl << endl;
    //    //test end

    //    SetCover setmodel(cross_num, nb_color, graph, population_sol, cross_sol_index);    //3.定义SetModel类，更新预处理的数据结构（模型返回的是所选集合的索引值），之后处理成挑选的集合和未加的节点，作为此类的返回值
    //    SetOfSol sets_sol = setmodel.solve();               //返回值为选中的集合列表和未被集合覆盖的节点列表
    //    CoverRemain cover_nodes(sets_sol, graph);              // 4.剩余节点加入的模型类，返回挑选的集合的数组
    //    bool flag = cover_nodes.model_solve();
    //    if (flag) {                                 //TODO:5.定义一个solution类，进行禁忌搜索，一定迭代步数内不能改进，将此解加入population_sol
    //        Solution cross_sol(graph.node_num(), nb_color);
    //        cross_sol.setInit(sets_sol);
    //        TabuSearch tabu(1000 * 100, graph, cross_sol);

    //        //tet
    //        cout << endl << "交叉之后的冲突边数目为：" << tabu.num_of_conflict() << endl << endl;
    //        //test end

    //        Solution new_sol = tabu.solve();
    //        if (new_sol.nb_conflict() == 0)return true;
    //        
    //        //test
    //        cout << endl << "交叉解禁忌搜索后的冲突边数目为：" << new_sol.nb_conflict() << endl << endl;
    //        //test end

    //        population_sol.emplace_back(new_sol);

    //    }
    //    
    //    //TODO：循环，继续执行2步骤
    //}
    //return false;
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
