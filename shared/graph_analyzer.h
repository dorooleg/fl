#pragma once
#include <vector>
#include <string>
#include <tuple>
#include "grammar.h"
#include "graph.h"

typedef std::vector<std::tuple<std::string, std::string, std::string>> vector_tuples;
vector_tuples analyze_graph(grammar& grammar, graph& graph);
