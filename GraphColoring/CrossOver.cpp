#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

Solution CrossOver::solve() {
    //TODO:1.�����ж�population_sol���Ƿ��г�ͻ��Ϊ0�Ľ⣬���У�ֱ�ӷ��ش˽�-----������һ����û��
    random_select_sol();     //2.�����ѡpopulation_sol�е�cross_num����-----��������������Set<>�У�ÿ��ѭ��֮ǰҪclear
    SetCover setmodel(cross_num, nb_color, weight_node, graph, population_sol, cross_sol_index);    //TODO:3.����SetModel�࣬����Ԥ��������ݽṹ��Model���ص�����ѡ���ϵ�����ֵ����֮�������ѡ�ļ��Ϻ�δ�ӵĽڵ㣬��Ϊ����ķ���ֵ
    SetOfSol sets_sol = setmodel.solve();
    //TODO:4.ʣ��ڵ�����ģ���࣬������ѡ�ļ��ϵ�����
    //TODO:5.����һ��solution�࣬���н���������һ�����������ڲ��ܸĽ������˽����population_sol
    //TODO��ѭ��������ִ��2����
}

void CrossOver::random_select_sol() {
    cross_sol_index.clear();
    for (int i = 0; i < cross_num; ) {
        int index = myrand.gen((int)population_sol.size() - 1);
        if (cross_sol_index.insert(index).second) { ++i; }
    }
}

//Solution CrossOver::solve() {
//    random_select_sol();          //��population_sol������������⣬������List��
//    prepross_sol();               //�������⴦������������б�����¼ÿ����ͻ�ڵ㣬SetSol
//    SetCoverModel cross_model(graph, set_of_crosssols, nb_color);          //����ѧģ�ʹ����ϸ������� (��ͻ�ڵ�Ȩ��--Ŀ��)
//    SetSol cross_sets = cross_model.solve();
//    //TODO�����δ�ڼ����еĽڵ㣬����ѧģ�͵�һ��Сͼ��ɫ����
//    //TODO���û��߲��þֲ����������½⣬�滻��Ⱥ�еĲ���ý⣬�������ý��������ֱ���������ʽ�
//    return population_sol[cross_solu_index[0]];
//}

//void CrossOver::random_select_sol() {      //�����ѡpopulation�е�������
//    cross_solu_index.push_back(myrand.gen((int)population_sol.size() - 1));
//    int j = myrand.gen((int)population_sol.size() - 1);
//    while (j == cross_solu_index[0]) j = myrand.gen((int)population_sol.size() - 1);
//    cross_solu_index.push_back(j);
//    //test
//    //cross_solu_index.push_back(0);
//}
//
////TODO:����������ʱ��Ҫ�޸�
////��Ⱥ�е����н���н���
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
