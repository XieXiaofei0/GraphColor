#include "utility.h"
#include <ctime>
#include "InputOutput.h"
#include "TabuSearch.h"
#include "ModelSolver.h"
#include "CrossOver.h"

using namespace std;
using namespace zjl_utility;
using namespace graph_coloring;

// �Խ����������в��ԣ�����������Ϊ max_iter
bool test_tabuSearch(string &file_name, int nb_color) {
    constexpr long long max_iter = 4500 * 10000;
    UGraph graph(file_name);
    Solution s1(graph.node_num(), nb_color);
    s1.randomInit();
    mylog << "��ɫ����" << s1.nb_color() <<= logsw_info;
    time_t t = time(0);
    myrand.setSeed(t);
    mylog << "������ӣ�" << t <<= logsw_info;
    TabuSearch tabu(max_iter, graph, s1);
    Solution sol = tabu.solve();
    sol.print();
    return sol.valid_solution();
}

// ��ģ�ͽ��в��ԣ�������ʱ��Ϊ60��
//bool test_modelSolver(string &file_name, int nb_color) {
//    UGraph graph(file_name);
//    mylog << "��ɫ����" << nb_color <<= logsw_info;
//    ModelSolver solver(graph, nb_color);
//    Solution sol = solver.solve();
//    sol.print();
//    return sol.valid_solution();
//}

//��ȷ����������ֲ�������������Ϊmax_iter,��Ⱥ����Ϊpopulation�������ĸ���Ϊcrossnum����ͻ�ڵ��Ȩ��ֵΪweight_con_node
bool test_crossOver(string &file_name, int nb_color) {
    constexpr long long max_iter = 45 * 100;
    constexpr int max_population = 200;
    constexpr int population = 10;
    constexpr int crossnum = 2;         //ע�⣺����ĸ������ܳ�����ɫ������ģ����ȡÿ�������ļ���������С������Ҫ���ݽ���ĸ�����
    UGraph graph(file_name);
    List<Solution> population_sl;
    List<Solution> population_sol;
    population_sl.reserve(population);
    population_sol.reserve(max_population);
    for (int i = 0; i < population; ++i) {
        mylog << "����" << i <<= logsw_info;
        population_sl.emplace_back(graph.node_num(), nb_color);
        population_sl[i].randomInit();
        mylog << "��ʼ�⹹�����" <<= logsw_info;
        mylog << "��ɫ����" << population_sl[i].nb_color() <<= logsw_info;
        time_t t = time(0);
        myrand.setSeed(t);
        mylog << "������ӣ�" << t <<= logsw_info;
        TabuSearch tabu(max_iter,graph,population_sl[i]);              //���ֲ������ķ���tabu��slֻ���˳�ʼ�����ɫ��
        population_sol.emplace_back(tabu.solve());                       //sol���ֲ����Ž⣨����-��ɫ������ɫ������ͻ��,�ڵ�ĳ�ͻ����
        //population_sol[i].print();
    }
    CrossOver crossover(graph, max_population, population_sol, crossnum, population_sl[0].nb_color());
    bool flag = crossover.solve();
    //sol.print();
    return true;
}

// ����֪�������л�׼���ԣ���ɫ�����Է�ΧΪ [best_known_colors, best_known_colors + max_color_relax]
void benchmark(bool test(string&, int)) {
    constexpr int max_color_relax = 10;
    List<string> instances_id{
        "125.1", "125.5", "125.9",
        "250.1", "250.5", "250.9",
        "500.1", "500.5", "500.9",
        "1000.1", "1000.5", "1000.9"
    };
    List<int> best_known_colors{
        5, 17, 44,
        8, 28, 72,
        12, 47, 126,
        20, 82, 222
    };
    int count = 0;
    //for (int i = 0; i < instances_id.size(); ++i) {
    //    for (int c_add = 0; c_add <= max_color_relax; ++c_add) {
    //        string file_name = "Instances/DSJC" + instances_id[i] + ".col";
    //        int nb_color = best_known_colors[i] + c_add;
    //        ++count;
    //        mylog << "��" << count << "�β���" <<= LogSwitch(1, 1, "BenchMark");
    //        if (test(file_name, nb_color)) {
    //            break;
    //        }
    //    }
    //}
    //xxf:��������500.1����
    for (int c_add = 0; c_add <= 5; ++c_add) {
        string file_name = "Instances/DSJC" + instances_id[6] + ".col";
        int nb_color = best_known_colors[6];
        ++count;
        mylog << "��" << count << "�β���" <<= LogSwitch(1, 1, "BenchMark");
        if (test(file_name, nb_color)) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    //benchmark(test_tabuSearch);
    //benchmark(test_modelSolver);
    benchmark(test_crossOver);
    system("pause");
    return 0;
}
