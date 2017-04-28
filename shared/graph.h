#pragma once
#include <vector>
#include <utility>
#include <fstream>

struct graph
{

    void load_file(const std::string& path)
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
    
    void generate_graph()
    {
        size_ = rand() % 100;
        size_t n = rand() % 1000;
        for (size_t i = 0; i < n; i++)
        {
            int text[] = { 'a', 'c', 'g', 't' };
            size_t a = rand() % 100;
            size_t b = rand() % 100;
            graph_[a].push_back(std::make_pair(b, text[rand() % 4]));
            graph_[b].push_back(std::make_pair(a, text[rand() % 4]));
        }
    }

    typedef std::vector<std::pair<int, char>>::iterator edge_iterator;
    typedef std::vector<std::vector<std::pair<int, char>>>::iterator iterator;
    
    edge_iterator from(size_t vertex)
    {
        return graph_[vertex].begin();
    }

    edge_iterator end(size_t vertex)
    {
        return graph_[vertex].end();
    }

    iterator begin()
    {
        return graph_.begin();
    }

    iterator end()
    {
        return graph_.end();
    }

	friend std::ostream& operator<<(std::ostream& os, const graph& graph)
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
    
    size_t size()
    {
        return size_;
    }

    std::vector<std::vector<std::pair<int, char>>>& get_graph()
    {
        return graph_;
    }

private:
    size_t size_;
    std::vector<std::vector<std::pair<int, char>>> graph_;
};
