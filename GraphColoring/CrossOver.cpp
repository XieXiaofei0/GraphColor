#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

Solution CrossOver::solve() {
    random_select_sol();          //��population_sol������������⣬������List��
    prepross_sol();               //�������⴦������������б�����¼ÿ����ͻ�ڵ㣬SetSol
    //TODO:����ѧģ�ʹ����ϸ������� (��ͻ�ڵ�Ȩ��--Ŀ��)
    //TODO�����δ�ڼ����еĽڵ㣬����ѧģ�͵�һ��Сͼ��ɫ����
    //TODO���û��߲��þֲ����������½⣬�滻��Ⱥ�еĲ���ý⣬�������ý��������ֱ���������ʽ�
    return population_sol[cross_solu_index[0]];
}

void CrossOver::random_select_sol() {      //�����ѡpopulation�е�������
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
