#pragma once
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>

struct graph
{

    void load_file(const std::string& path);
    
    void generate_graph();

    typedef std::vector<std::pair<int, char>>::iterator edge_iterator;
    typedef std::vector<std::vector<std::pair<int, char>>>::iterator iterator;
    
    edge_iterator from(size_t vertex);

    edge_iterator end(size_t vertex);

    iterator begin();

    iterator end();

	friend std::ostream& operator<<(std::ostream& os, const graph& graph);
    
    size_t size();

    std::vector<std::vector<std::pair<int, char>>>& get_graph();

private:
    size_t size_;
    std::vector<std::vector<std::pair<int, char>>> graph_;
};
