#include "graph_analyzer.h"

vector_tuples analyze_graph(grammar& grammar, graph& graph)
{
    std::vector<std::vector<std::unordered_set<std::string>>> matrix(graph.size());
    vector_tuples result;
    for (size_t i = 0; i < graph.size(); i++)
        matrix[i].resize(graph.size());
    auto vg = graph.get_graph();
    for (size_t i = 0; i < graph.size(); i++)
        for (const auto& v : vg[i]) 
        {
            std::string rev;
            rev += v.second;
            auto t = grammar.find_terminal(rev);
            for (auto & aba : t)
            {
                matrix[i][v.first].insert(aba.get_value());
            }
        }
    
     
    bool visited = true;
    while (visited)
    {
        visited = false;
        for (size_t i = 0; i < graph.size(); i++)
        {
            std::cout << "i = " << i << std::endl; 
            for (size_t j = 0; j < graph.size(); j++)
                for (size_t k = 0; k < graph.size(); k++)
                {
                    if (k == i || k == j || i == j)
                        continue;
                    auto& v1 = matrix[i][k];
                    auto& v2 = matrix[k][j];
                    for (const auto& t1 : v1)
                        for (const auto& t2 : v2)
                        {
                            auto values = grammar.find_no_terminal(t1, t2);
                            for (auto & aba : values)
                            {
                                if (!matrix[i][j].count(aba.get_value()))
                                {
                                    visited = true;
                                    matrix[i][j].insert(aba.get_value());
                                }
                            }
                        }
                }
        }
    }

    for (size_t i = 0; i < graph.size(); i++)
        for (size_t j = 0; j < graph.size(); j++)
            for (const auto & v : matrix[i][j])
                if (i < j) 
                    result.push_back(std::make_tuple(std::to_string(i), std::to_string(j), v));

    return result;
}
