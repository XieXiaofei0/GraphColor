#pragma once
#ifndef GRAPHCOLORING_COMMON_H
#define GRAPHCOLORING_COMMON_H
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>

namespace graph_coloring {

template<typename T>
using List = std::vector<T>;

template<typename T>
using Set = std::set<T>;

template<typename Key, typename Val>
using Map = std::map<Key, Val>;

template<typename Key, typename Val>
using HashMap = std::unordered_map<Key, Val>;

using String = std::string;

}

#endif // !GRAPHCOLORING_COMMON_H
