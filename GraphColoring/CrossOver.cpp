#include "CrossOver.h"


using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

//Solution 
bool CrossOver::solve() {
    //TODO:1.�����ж�population_sol���Ƿ��г�ͻ��Ϊ0�Ľ⣬���У�ֱ�ӷ��ش˽�-----������һ����û��
    random_select_sol();     //2.�����ѡpopulation_sol�е�cross_num����-----��������������Set<>�У�ÿ��ѭ��֮ǰҪclear
    SetCover setmodel(cross_num, nb_color, graph, population_sol, cross_sol_index);    //3.����SetModel�࣬����Ԥ��������ݽṹ��ģ�ͷ��ص�����ѡ���ϵ�����ֵ����֮�������ѡ�ļ��Ϻ�δ�ӵĽڵ㣬��Ϊ����ķ���ֵ
    SetOfSol sets_sol = setmodel.solve();               //����ֵΪѡ�еļ����б��δ�����ϸ��ǵĽڵ��б�
    CoverRemain cover_nodes(sets_sol, graph);              //TODOL:������ 4.ʣ��ڵ�����ģ���࣬������ѡ�ļ��ϵ�����
    bool flag = cover_nodes.model_solve();
    //TODO:5.����һ��solution�࣬���н���������һ�����������ڲ��ܸĽ������˽����population_sol
    //TODO��ѭ��������ִ��2����
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
