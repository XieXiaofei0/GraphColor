#include "InputOutput.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "utility.h"

using namespace std;
using namespace zjl_utility;

namespace graph_coloring {

UGraph::UGraph(const String & path) {
    ifstream ifs;
    ifs.open(path);
    if (!ifs.is_open()) {
        mylog << "�޷����ļ�" << path <<= logsw_error;
        return;
    }
    string line;
    int count = 0;
    while (getline(ifs, line)) {
        if (line.size() && line[0] == 'p') {
            istringstream iss(line);
            iss >> line >> line;
            iss >> nb_node_ >> nb_edge_;
            break;
        }
        ++count;
    }
    char c;
    int node1, node2;
    Set<int> node_set;
    while (ifs >> c >> node1 >> node2) {
        if (node1 == node2) {
            mylog << "��" << count << "�вγ���ָ���Լ��ı�" <<= logsw_error;
            continue;
        }
        if (node1 > 0 && node2 > 0 && node1 <= nb_node_ && node2 <= nb_node_) {
            Edge edge;
            if (node1 < node2) {
                edge.st = node1 - 1;
                edge.ed = node2 - 1;
            } else {
                edge.st = node2 - 1;
                edge.ed = node1 - 1;
            }
            edges_.push_back(edge);
            node_set.insert(node1);
            node_set.insert(node2);
        } else {
            mylog << "��" << count << "�в�������" <<= logsw_error;
        }
        ++count;
    }
    // ȥ���ظ��ı�
    sort(edges_.begin(), edges_.end(), [](Edge &lhs, Edge &rhs) {
        return lhs.st < rhs.st || (lhs.st == rhs.st && lhs.ed < rhs.ed);
    });
    int index = 1;
    for (int i = 1; i < edges_.size(); ++i) {
        if (edges_[i].st != edges_[i - 1].st ||
            edges_[i].ed != edges_[i - 1].ed) {
            edges_[index++] = edges_[i];
        }
    }
    edges_.resize(index);
    if (edges_.size() != nb_edge_) {
        mylog << "�ļ��б���Ϊ��" << nb_edge_ <<
            " ʵ�ʱ���Ϊ:" << edges_.size() <<= logsw_info;
        nb_edge_ = edges_.size();
    }
    if (node_set.size() != nb_node_) {
        mylog << "�ļ��ж�����Ϊ:" << nb_node_ <<
            "ʵ�ʶ�����Ϊ:" << node_set.size() <<= logsw_info;
        nb_node_ = node_set.size();
    }
    neighbor_node_.resize(nb_node_);
    neighbor_edge_.resize(nb_node_);
    // �����ھ�
    for (int e = 0; e < nb_edge_; ++e) {
        neighbor_node_[edges_[e].st].push_back(edges_[e].ed);
        neighbor_node_[edges_[e].ed].push_back(edges_[e].st);
        neighbor_edge_[edges_[e].st].push_back(e);
        neighbor_edge_[edges_[e].ed].push_back(e);
    }
    mylog << "�����ļ�" << path <<= logsw_info;
    mylog << "ͼ�нڵ���ĿΪ��" << nb_node_ 
        << " �ߵ���ĿΪ:" << nb_edge_ <<= logsw_info;
}

void Solution::randomInit() {
    for (int i = 0; i < nb_node_; ++i) {
        node_colors_[i] = myrand.gen(nb_color_ - 1);
    }
}

void Solution::setInit(const SetOfSol &_setsol) {
    for (int i = 0; i < _setsol.color_set.size(); ++i) {
        for (auto j : _setsol.color_set[i]) {
            node_colors_[j] = i;
        }
    }
}

void Solution::print() {
    mylog << "��ĳ�ͻ����ĿΪ��" << nb_conflict_ <<= logsw_info;
    //mylog << "�ڵ�->��ɫֵ�������£�" <<= logsw_info;
    //for (int i = 0; i < node_colors_.size(); ++i) {
    //    mylog << i + 1 << "->" << node_colors_[i] + 1 << " ";
    //}
    //mylog <<= logsw_info;
}

}
