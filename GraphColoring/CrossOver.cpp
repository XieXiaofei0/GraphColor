#include "CrossOver.h"


using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

//Solution 
bool CrossOver::solve() {
    //����һ��ֱ����������н���
    //random_select_sol();
    cross_sol_index.clear();
    cross_sol_index.insert(0);
    cross_sol_index.insert(1);

    //test
    mylog << "ѡ���Ľ����ĳ�ͻ����  " << population_sol[0].nb_conflict() << "     " << population_sol[1].nb_conflict() <<= logsw_info;
    //cout << endl;
    //cout << "ѡ���Ľ����ĳ�ͻ����" << endl;
    //for (auto i : cross_sol_index) {
    //     cout << "             " << i << "     ��ͻ����" << population_sol[i].nb_conflict() << endl;
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
        mylog << "����֮��ĳ�ͻ����ĿΪ��" << tabu.num_of_conflict() <<= logsw_info;
        //test end

        Solution new_sol = tabu.solve();
        if (new_sol.nb_conflict() == 0)return true;
            
        //test
        mylog << "��������������ĳ�ͻ����ĿΪ��" << new_sol.nb_conflict() <<= logsw_info;
        //test end
    }
    return false;
    //��������������Ӹ���population�еĽ�
    //TODO:1.�����ж�population_sol���Ƿ��г�ͻ��Ϊ0�Ľ⣬���У�ֱ�ӷ��ش˽�-----������һ����û��
    //while (population_sol.size() < 100) {
    //    random_select_sol();     //2.�����ѡpopulation_sol�е�cross_num����-----��������������Set<>�У�ÿ��ѭ��֮ǰҪclear

    //    //test:         TODO:�����ѡ�Ľ����ĳ�ͻ��
    //    cout << endl;
    //    cout << "ѡ���Ľ����ĳ�ͻ����" << endl;
    //    for (auto i : cross_sol_index) {
    //        cout << "             " << i << "     ��ͻ����" << population_sol[i].nb_conflict() << endl;
    //    }
    //    cout << endl << endl;
    //    //test end

    //    SetCover setmodel(cross_num, nb_color, graph, population_sol, cross_sol_index);    //3.����SetModel�࣬����Ԥ��������ݽṹ��ģ�ͷ��ص�����ѡ���ϵ�����ֵ����֮�������ѡ�ļ��Ϻ�δ�ӵĽڵ㣬��Ϊ����ķ���ֵ
    //    SetOfSol sets_sol = setmodel.solve();               //����ֵΪѡ�еļ����б��δ�����ϸ��ǵĽڵ��б�
    //    CoverRemain cover_nodes(sets_sol, graph);              // 4.ʣ��ڵ�����ģ���࣬������ѡ�ļ��ϵ�����
    //    bool flag = cover_nodes.model_solve();
    //    if (flag) {                                 //TODO:5.����һ��solution�࣬���н���������һ�����������ڲ��ܸĽ������˽����population_sol
    //        Solution cross_sol(graph.node_num(), nb_color);
    //        cross_sol.setInit(sets_sol);
    //        TabuSearch tabu(1000 * 100, graph, cross_sol);

    //        //tet
    //        cout << endl << "����֮��ĳ�ͻ����ĿΪ��" << tabu.num_of_conflict() << endl << endl;
    //        //test end

    //        Solution new_sol = tabu.solve();
    //        if (new_sol.nb_conflict() == 0)return true;
    //        
    //        //test
    //        cout << endl << "��������������ĳ�ͻ����ĿΪ��" << new_sol.nb_conflict() << endl << endl;
    //        //test end

    //        population_sol.emplace_back(new_sol);

    //    }
    //    
    //    //TODO��ѭ��������ִ��2����
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
