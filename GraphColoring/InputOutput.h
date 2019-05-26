#pragma once
#ifndef GRAPH_COLORING_INPUTOUTPUT_H
#define GRAPH_COLORING_INPUTOUTPUT_H
#include "Common.h"

namespace graph_coloring {

struct Edge {
    int st = -1;
    int ed = -1;
    //double weight;
};

// 无向图，采用邻接表储存
class UGraph {
public:
    static const int invalidId() { return -1; }
    UGraph() :nb_node_(0), nb_edge_(0) {};
    UGraph(const UGraph &other) : neighbor_node_(other.neighbor_node_), neighbor_edge_(other.neighbor_edge_),
        edges_(other.edges_), nb_node_(other.nb_node_), nb_edge_(other.nb_edge_) {};
    UGraph(const String &path);

    void operator=(const UGraph &other) {
        neighbor_node_ = other.neighbor_node_;
        neighbor_edge_ = other.neighbor_edge_;
        edges_ = other.edges_;
        nb_node_ = other.nb_node_;
        nb_edge_ = other.nb_edge_;
    }
    
    const List<int>& operator[] (int node) const { return neighbor_node_.at(node); }
    const List<int>& neighbor_node(int node) const { return neighbor_node_.at(node); }

    const List<int>& neighbor_edge(int node) const { return neighbor_edge_.at(node); }
    const Edge& edge(int edge_id) const { return edges_.at(edge_id); }

    size_t node_num() const { return nb_node_; }
    size_t edge_num() const { return nb_edge_; }
    double density() const { return static_cast<double>(nb_edge_) / static_cast<double>(nb_node_) * 2.0; }
private:
    List<List<int>> neighbor_node_;        //xxf:保存与所有节点相连的节点
    List<List<int>> neighbor_edge_;       //xxf:保存与所有节点相连的边
    List<Edge> edges_;
    size_t nb_node_;
    size_t nb_edge_;
};

class Solution {
public:
    Solution(int nb_node = 0, int nb_color = 0) :
        nb_node_(nb_node), nb_color_(nb_color), nb_conflict_(INT_MAX) {
        node_colors_.resize(nb_node, -1);
    }
    Solution(int nb_color, int nb_conflict, List<int> &node_colors): nb_node_(node_colors.size()),
        nb_color_(nb_color), nb_conflict_(nb_conflict), node_colors_(node_colors) {}
    //xxf
    Solution(int nb_color, int nb_conflict, List<int> &node_colors,const List<int> &node_conflict):
        nb_node_(node_colors.size()),nb_color_(nb_color), nb_conflict_(nb_conflict), node_colors_(node_colors)
    {
        is_conflict_node_.reserve(node_conflict.size());
        for (auto i : node_conflict) {
            if (i == 0)is_conflict_node_.push_back(false);
            else is_conflict_node_.push_back(true);
        }
    }
    //end xxf
    Solution(Solution &other) :nb_color_(other.nb_color_), nb_node_(other.nb_node_),
        nb_conflict_(other.nb_conflict_), node_colors_(other.node_colors_),is_conflict_node_(other.is_conflict_node_) {}
    Solution(Solution &&other) :nb_color_(other.nb_color_), nb_node_(other.nb_node_),
        nb_conflict_(other.nb_conflict_), node_colors_(std::move(other.node_colors_)), is_conflict_node_(std::move(other.is_conflict_node_)) {}
    Solution& operator= (Solution &rhs) {
        nb_color_ = rhs.nb_color_;
        nb_node_ = rhs.nb_node_;
        nb_conflict_ = rhs.nb_conflict_;
        node_colors_ = rhs.node_colors_;
        is_conflict_node_ = rhs.is_conflict_node_;
        return *this;
    }
    Solution& operator= (Solution &&rhs) {
        nb_color_ = rhs.nb_color_;
        nb_node_ = rhs.nb_node_;
        nb_conflict_ = rhs.nb_conflict_;
        node_colors_ = std::move(rhs.node_colors_);
        is_conflict_node_ = std::move(rhs.is_conflict_node_);
        return *this;
    }

    int& operator[] (int node) { return node_colors_[node]; }
    bool operator< (Solution &rhs) { return this->nb_conflict_ < rhs.nb_conflict_; }
    const List<int>& node_colors() const { return node_colors_; }
    int nb_color() const { return nb_color_; }
    bool valid_solution() const { return nb_conflict_ == 0; }

    void set_nb_conflict(int num) { nb_conflict_ = num; }
    int nb_conflict() { return nb_conflict_; }
    const List<bool>& is_conflict_node() const { return is_conflict_node_; }

    // 为节点随机分配颜色
    void randomInit();
    // 打印结果
    void print();
private:
    int nb_color_;            // 可用颜色数目
    int nb_node_;             // 要着色的顶点数目
    int nb_conflict_;         // 冲突边的数目
    List<int> node_colors_;   // 为每个节点染一种颜色
    //xxf
    List<bool> is_conflict_node_;   //记录节点是否是冲突节点
    //xxf end
};

}

#endif // !GRAPH_COLORING_INPUTOUTPUT_H
