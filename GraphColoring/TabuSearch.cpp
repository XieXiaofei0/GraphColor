#include "TabuSearch.h"
#include "utility.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

LogSwitch logsw_tabu(1, 1, "PINCOL_PT");

#define NONE -1

TabuSearch::TabuSearch(long long max_iter, const UGraph & _graph, const Solution & _init_sol) :
    MAXSTEPS(max_iter), graph(_graph), node_value(_init_sol.node_colors()), NB_VALUE(_init_sol.nb_color()) {   //xxf：done
    NB_NODE = graph.node_num();
    NB_EDGE = graph.edge_num();
    local_best = node_value;
    init();
}

Solution TabuSearch::solve() {
    int best_obj = NB_EDGE;      //xxf:初始化best_obj为边的数目
    List<int> nodeColor;
    nodeColor.resize(2);
    while (cur_iter < MAXSTEPS) {
        if (EDGE_STACK.size() == 0)
            break;
        if (PRM_MOVE_STACK.size() > 0) {       //xxf:每找到一步有希望减少冲突的动作，nbprm+1
            choose_best_prm_move(nodeColor);
            nbprm++;
        } else if (ZRO_MOVE_STACK.size() > 0 && choose_best_zero_move(nodeColor)) {
            nbzero++;
        } else {
            choose_node_color(nodeColor);
            nbwalk++;
        }
        flip_and_update_score(nodeColor[0], nodeColor[1],
            node_value[nodeColor[0]]);
        adaptNoise();
        if (EDGE_STACK.size() < best_obj) {
            best_obj = EDGE_STACK.size();
            local_best = node_value;
            //mylog << "a better obj:" << best_obj <<= logsw_tabu;
        }
        ++cur_iter;
    }
    mylog << "\n总迭代步数：" << cur_iter 
        << "步\n随机游走概率：" << (double)nbwalk / (double)cur_iter * 100
        << "%\nZERO_MOVE概率：" << (double)nbzero / (double)cur_iter * 100
        << "%\nPRM_MOVE概率：" << (double)nbprm / (double)cur_iter * 100 << "% " <<= logsw_tabu;
    if (best_obj == get_conflict_edge_num(local_best)) {
        mylog << "目标函数值正确！" <<= logsw_tabu;
    } else {
        mylog << "目标函数值错误！" <<= logsw_error;
    }
    return Solution(NB_VALUE, best_obj, local_best, node_conflict);
}

void TabuSearch::update_prm_moves() {    //xxf:刷新PRM_MOVE_STACK数组
    int i, j;
    for (i = 0; i < PRM_MOVE_STACK.size(); i += 2)
        if (node_score[PRM_MOVE_STACK[i]][PRM_MOVE_STACK[i + 1]] <= 0)
            break;
    for (j = i + 2; j < PRM_MOVE_STACK.size(); j += 2) {
        if (node_score[PRM_MOVE_STACK[j]][PRM_MOVE_STACK[j + 1]] > 0) {
            PRM_MOVE_STACK[i] = PRM_MOVE_STACK[j];
            PRM_MOVE_STACK[i + 1] = PRM_MOVE_STACK[j + 1];
            i += 2;
        }
    }
    PRM_MOVE_STACK.resize(i);
}

void TabuSearch::update_zero_moves() {   //刷新ZRO_MOVE_STACK数组
    int i, j;
    for (i = 0; i < ZRO_MOVE_STACK.size(); i += 2)
        if (!is_zero_move(ZRO_MOVE_STACK[i], ZRO_MOVE_STACK[i + 1]))
            break;
    for (j = i + 2; j < ZRO_MOVE_STACK.size(); j += 2) {
        if (is_zero_move(ZRO_MOVE_STACK[j], ZRO_MOVE_STACK[j + 1])) {
            ZRO_MOVE_STACK[i] = ZRO_MOVE_STACK[j];
            ZRO_MOVE_STACK[i + 1] = ZRO_MOVE_STACK[j + 1];
            i += 2;
        }
    }
    ZRO_MOVE_STACK.resize(i);
}

void TabuSearch::flip_and_update_score(int node, int new_color, int old_color) {     //xxf:做一个动作并更新相关数据结构
    node_value[node] = new_color;
    flip_time[node][new_color] = cur_iter;
    leave_time[node][old_color] = cur_iter;
    class_size[old_color]--;
    class_size[new_color]++;
    tabu[node] = old_color;
    int new_score = node_score[node][new_color];
    for (int c = 0; c < NB_VALUE; c++) {
        node_score[node][c] = node_score[node][c] - new_score;
    }
    if (new_score != 0) {
        node_conflict[node] -= new_score;
        for (int c = 0; c < NB_VALUE; c++)
            push_zero_move(node, c);
    }
    for (int n = 0; n < graph.neighbor_node(node).size(); ++n) {
        int neibor = graph.neighbor_node(node)[n];
        tabu[neibor] = NONE;
        if (node_value[neibor] == old_color) {
            node_conflict[neibor]--;
            int last_conflicting_edge = EDGE_STACK.back();
            EDGE_STACK.pop_back();
            int e = graph.neighbor_edge(node)[n];
            int index = index_in_EDGE_STACK[e];
            if (index < EDGE_STACK.size()) {
                EDGE_STACK[index] = last_conflicting_edge;
                index_in_EDGE_STACK[last_conflicting_edge] = index;
            }
            for (int c = 0; c < NB_VALUE; c++) {
                if (c != old_color) {
                    node_score[neibor][c]--;
                    push_zero_move(neibor, c);
                }
            }
            node_score[neibor][new_color]--;
            push_zero_move(neibor, new_color);
        } else if (node_value[neibor] == new_color) {
            node_conflict[neibor]++;
            int e = graph.neighbor_edge(node)[n];
            index_in_EDGE_STACK[e] = EDGE_STACK.size();
            EDGE_STACK.push_back(e);
            for (int c = 0; c < NB_VALUE; c++) {
                if (c != new_color) {
                    node_score[neibor][c]++;
                    push_move_stack(neibor, c);
                }
            }
            node_score[neibor][old_color]++;
            push_move_stack(neibor, old_color);
        } else {
            node_score[neibor][old_color]++;
            push_move_stack(neibor, old_color);
            node_score[neibor][new_color]--;
            push_zero_move(neibor, new_color);
        }
    }
    update_prm_moves();
    update_zero_moves();
}

void TabuSearch::random_walk(int conflicting_edge, List<int>& nodeColor) {    //xxf:当概率小于随机游走最小概率时，找一个动作
    int choice, node, c, current_color;
    choice = myrand.gen(2 * NB_VALUE - 3);
    if (choice < NB_VALUE - 1)
        node = graph.edge(conflicting_edge).st;
    else {
        node = graph.edge(conflicting_edge).ed;
        choice = choice - NB_VALUE + 1;
    }
    current_color = node_value[node];
    for (c = 0; c < NB_VALUE; c++) {
        if (c != current_color) {
            if (choice == 0) {
                nodeColor[0] = node;
                nodeColor[1] = c;
                return;
            } else
                choice--;
        }
    }
}

void TabuSearch::choose_node_color(List<int>& nodeColor) {     //xxf:随机游走选择第一好动作或第二好动作
    int node, i, conflicting_edge, nb, c, energy, max_nb = -NB_EDGE,
        second_max = NONE, second_best_node = NONE, second_color = NONE,
        best_node = NONE, best_color = NONE, chosen_node, chosen_color,
        conflicting_edge_index, current_color, best_node_energy = NB_NODE,
        second_node_energy = NB_NODE;
    long long recent_time = 0, ftime = 0, best_flip_time = MAXSTEPS,
        second_flip_time = MAXSTEPS;
    conflicting_edge_index = myrand.gen((int)(EDGE_STACK.size() - 1));   // choose a conflict edge random  ++
    conflicting_edge = EDGE_STACK[conflicting_edge_index];
    if (myrand.gen(99) < LNOISE) {                //random walk by some posibility  ++   //xxf:随机产生一个数判断是否小于随机游走最小概率？
        random_walk(conflicting_edge, nodeColor);
        return;
    }
    for (i = 0; i < 2; i++) {
        if (i == 0)
            node = graph.edge(conflicting_edge).st;
        else
            node = graph.edge(conflicting_edge).ed;
        current_color = node_value[node];
        energy = node_conflict[node];
        for (c = 0; c < NB_VALUE; c++) {
            if (c != current_color && tabu[node] != c) {   //color is legal  ++
                nb = node_score[node][c];
                ftime = leave_time[node][c];
                if (nb > max_nb                        //find the best move  ++
                    || (nb == max_nb
                        && (energy > best_node_energy
                            || (energy == best_node_energy
                                && ftime < best_flip_time)))) {
                    second_best_node = best_node;
                    second_color = best_color;
                    second_max = max_nb;
                    second_flip_time = best_flip_time;
                    second_node_energy = best_node_energy;
                    best_node = node;
                    best_color = c;
                    max_nb = nb;
                    best_flip_time = ftime;
                    best_node_energy = energy;
                } else if (nb > second_max              //find the second best move  ++
                    || (nb == second_max
                        && (energy > second_node_energy
                            || (energy == second_node_energy
                                && ftime < second_flip_time)))) {
                    second_max = nb;
                    second_flip_time = ftime;
                    second_node_energy = energy;
                    second_best_node = node;
                    second_color = c;
                }
                if (ftime > recent_time)
                    recent_time = ftime;
            }
        }
    }
    if (recent_time == best_flip_time) {
        if (myrand.gen(99) < SNOISE) {
            chosen_node = second_best_node;
            chosen_color = second_color;
        } else {
            chosen_node = best_node;
            chosen_color = best_color;
        }
    } else {
        chosen_node = best_node;
        chosen_color = best_color;
    }
    nodeColor[0] = chosen_node;
    nodeColor[1] = chosen_color;
}

void TabuSearch::init() {     //xxf:done；初始化class_size,EDGE_STACK,冲突表，PRM_MOVE_STACK和 ZRO_MOVE_STACK
    static constexpr int invTheta = 5;
    // 初始化每种颜色含有的顶点数目。
    class_size.resize(NB_VALUE, 0);
    for (int node = 0; node < NB_NODE; node++) {
        class_size[node_value[node]]++;
    }
    // 初始化每个顶点冲突数目和禁忌表，记录所有冲突边。
    EDGE_STACK.reserve(NB_EDGE);
    index_in_EDGE_STACK.resize(NB_EDGE, NONE);
    node_conflict.resize(NB_NODE, 0);
    tabu.resize(NB_NODE, NONE);
    for (int i = 0; i < NB_EDGE; i++) {
        if (node_value[graph.edge(i).st] == node_value[graph.edge(i).ed]) {
            index_in_EDGE_STACK[i] = EDGE_STACK.size();
            EDGE_STACK.push_back(i);
        }
    }
    // 初始化冲突表，记录所有PRM_MOVE和ZRO_MOVE。
    node_score.resize(NB_NODE, List<int>(NB_VALUE, 0));   //xxf:冲突表
    flip_time.resize(NB_NODE, List<long long>(NB_VALUE, 0));
    leave_time.resize(NB_NODE, List<long long>(NB_VALUE, 0));
    PRM_MOVE_STACK.reserve(NB_NODE*NB_VALUE);
    ZRO_MOVE_STACK.reserve(NB_NODE*NB_VALUE);
    for (int node = 0; node < NB_NODE; node++) {
        for (int neibor : graph[node])    //xxf:与node相连的节点
            node_score[node][node_value[neibor]]++;
        int nb_conflicts = node_score[node][node_value[node]];     //xxf:记录节点node的冲突边
        node_conflict[node] = nb_conflicts;    //xxf:记录节点冲突边数目
        for (int c = 0; c < NB_VALUE; c++) {                                //xxf:更新PRM_MOVE_STACK和ZRO_MOVE_STACK
            node_score[node][c] = nb_conflicts - node_score[node][c];
            if (node_score[node][c] > 0) {
                PRM_MOVE_STACK.push_back(node);
                PRM_MOVE_STACK.push_back(c);
            } else if (is_zero_move(node, c)) {
                ZRO_MOVE_STACK.push_back(node);
                ZRO_MOVE_STACK.push_back(c);
            }
        }
    }
    // 初始化噪声和各种标志参数
    lastAdaptFlip = 0;
    lastBest = EDGE_STACK.size();
    NOISE = 0;
    LNOISE = 0;
    AdaptLength = NB_EDGE / invTheta;
}

// 当找到一个更好的解时，降低随机游走概率，否则逐渐增加随机游走的概率
void TabuSearch::adaptNoise() {   //xxf:done
    static constexpr int invPhi = 10;
    if (EDGE_STACK.size() < lastBest) {
        NOISE -= (int)(NOISE / (2 * invPhi));
        LNOISE = (int)NOISE / 10;
        lastAdaptFlip = cur_iter;
        lastBest = EDGE_STACK.size();
    } else if ((cur_iter - lastAdaptFlip) > AdaptLength) {
        NOISE += (int)((100 - NOISE) / invPhi);
        LNOISE = (int)NOISE / 10;
        lastAdaptFlip = cur_iter;
        lastBest = EDGE_STACK.size();
    }
}

bool TabuSearch::choose_best_zero_move(List<int>& nodeColor) {      //xxf:选择最好的使冲突数变化为0的动作 （函数具体含义）
    int i, node, color, score = 0;
    int best_conf_index = -1, best_conf_score = 0, best_conf_size = 0;
    int best_zero_index = -1, best_zero_score = 0, best_zero_size = 0;
    long long best_zero_time = MAXSTEPS, best_conf_time = MAXSTEPS;
    for (i = 0; i < ZRO_MOVE_STACK.size(); i += 2) {
        node = ZRO_MOVE_STACK[i];
        color = ZRO_MOVE_STACK[i + 1];
        score = class_size[color] - class_size[node_value[node]];     //xxf:score=新颜色color包含的顶点数目-旧颜色node_value[node]包含的顶点数目
        if (score >= 0) {    //xxf:为什么只有socre>=0时才选择
            if (node_conflict[node] == 0) {    //xxf:原节点node的冲突边为0  隐含信息：node换成color颜色冲突边也为0
                score = class_size[color];    //xxf:新颜色color的节点数目
                if (score > best_zero_score) {      //xxf:找最大的score
                    best_zero_index = i;
                    best_zero_score = score;
                    best_zero_size = class_size[node_value[node]];
                    best_zero_time = flip_time[node][color];
                } else if (score == best_zero_score
                    && class_size[node_value[node]] < best_zero_size) {   //xxf:找最小的class_size[node_value[node]],节点node所着颜色包含的节点数目
                    best_zero_index = i;
                    best_zero_size = class_size[node_value[node]];
                    best_zero_time = flip_time[node][color];
                } else if (score == best_zero_score
                    && class_size[node_value[node]] == best_zero_size
                    && flip_time[node][color] < best_zero_time) {        //xxf:找节点node着color颜色的最远时间
                    best_zero_index = i;
                    best_zero_time = flip_time[node][color];
                }
            } else if (node_conflict[node] == 1) {   //xxf:节点node的冲突为1
                if (score > best_conf_score) {
                    best_conf_index = i;
                    best_conf_score = score;
                    best_conf_size = class_size[color];         //xxf:新颜色color的节点数目
                    best_conf_time = flip_time[node][color];
                } else if (score == best_conf_score
                    && class_size[color] > best_conf_size) {       //xxf:找最大的新颜色的节点数目
                    best_conf_index = i;
                    best_conf_size = class_size[color];
                    best_conf_time = flip_time[node][color];
                } else if (score == best_conf_score
                    && class_size[color] == best_conf_size
                    && flip_time[node][color] < best_conf_time) {      //xxf:找节点node着color颜色的最远时间
                    best_conf_index = i;
                    best_conf_time = flip_time[node][color];
                }
            }
        }
    }
    if (best_zero_index > -1) {
        nodeColor[0] = ZRO_MOVE_STACK[best_zero_index];
        nodeColor[1] = ZRO_MOVE_STACK[best_zero_index + 1];
        return true;
    } else if (best_conf_index > -1 && myrand.gen(99) < ZNOISE) {    //xxf:为什么加这个概率
        nodeColor[0] = ZRO_MOVE_STACK[best_conf_index];
        nodeColor[1] = ZRO_MOVE_STACK[best_conf_index + 1];
        return true;
    } else {
        return false;
    }
}

bool TabuSearch::choose_best_prm_move(List<int>& nodeColor) {   //xxf:done，从有希望的动作中找最老的动作，根据leave_time,节点i被去掉颜色j的时刻
    int i, index = -1;
    long long ftime = MAXSTEPS, time = 0;
    if (PRM_MOVE_STACK.size() == 2) {   // 这时候没得选
        nodeColor[0] = PRM_MOVE_STACK[0];
        nodeColor[1] = PRM_MOVE_STACK[1];
        return true;
    }
    // 找最老的动作
    for (i = 0; i < PRM_MOVE_STACK.size(); i += 2) {
        //time = flip_time[PRM_MOVE_STACK[i]][PRM_MOVE_STACK[i + 1]];
        time = leave_time[PRM_MOVE_STACK[i]][PRM_MOVE_STACK[i + 1]];
        if (time < ftime) {             //xxf:init初始化之后，leave_time都为0？
            index = i;
            ftime = time;
        }
    }
    if (index == -1) {
        return false;
    }
    nodeColor[0] = PRM_MOVE_STACK[index];
    nodeColor[1] = PRM_MOVE_STACK[index + 1];
    return true;
}

int TabuSearch::get_conflict_edge_num(List<int>& sol) {   //xxf:判断函数值是否正确
    int count = 0;
    for (int i = 0; i < NB_EDGE; ++i) {
        if (sol[graph.edge(i).st] == sol[graph.edge(i).ed]) {
            count++;
        }
    }
    return count;
}

}
