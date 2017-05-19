#include "graph.h"

void graph::load_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Can't open file" << std::endl;
        exit(EXIT_FAILURE);
    }
    size_t n = 0;
    file >> n;
    graph_.resize(n);
    size_ = n;
    file >> n;
    for (size_t i = 0; i < n; i++)
    {
        size_t a = 0;
        size_t b = 0;
        char c = '\0';
        file >> a >> b >> c;
        graph_[a].push_back(std::make_pair(b, c));
        graph_[b].push_back(std::make_pair(a, c));
    }
}

void graph::generate_graph()
{
    size_ = 50;//rand() % 100;
    size_t n = 1000;//rand() % 1000;
    graph_.resize(size_);
    
    for (size_t i = 0; i < n; i++)
    {
        int text[] = { 'a', 'c', 'g', 't' };
        size_t a = rand() % size_;
        size_t b = rand() % size_;
        graph_[a].push_back(std::make_pair(b, text[rand() % 4]));
        graph_[b].push_back(std::make_pair(a, text[rand() % 4]));
    }
}

graph::edge_iterator graph::from(size_t vertex)
{
    return graph_[vertex].begin();
}

graph::edge_iterator graph::end(size_t vertex)
{
    return graph_[vertex].end();
}

graph::iterator graph::begin()
{
    return graph_.begin();
}

graph::iterator graph::end()
{
    return graph_.end();
}

std::ostream& operator<<(std::ostream& os, const graph& graph)
{
    for (size_t i = 0; i < graph.graph_.size(); i++)
    {
        for (const auto& v : graph.graph_[i])
        {
            os << i << " " << v.first << " " << v.second << std::endl;
        }
    }
    return os;
}

size_t graph::size()
{
    return size_;
}

std::vector<std::vector<std::pair<int, char>>>& graph::get_graph()
{
    return graph_;
}
