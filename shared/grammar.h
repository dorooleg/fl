#pragma once
#include "expression.h"
#include "non_terminal.h"
#include "terminal.h"
#include "graph.h"
#include <fstream>
#include <tuple>
#include <utility>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <unordered_map>

class grammar
{
public:
	typedef std::vector<std::pair<non_terminal, std::vector<expression>>> container;

	grammar();

	void push_back(const non_terminal& lvalue, const std::vector<expression>& expressions);

	void load_file(const std::string& path_to_file);

	void remove_long_rule();

	void remove_multi_terminals();

	void remove_eps();

	std::vector<std::shared_ptr<non_terminal>> get_eps_deducible();

	void remove_unborn();

	void remove_unattainable();

	void remove_useless();

	void remove_chains_rules();

	void to_cnf();

	typedef std::map<std::string, std::vector<std::vector<bool>>> matrix_t;
	matrix_t cyk(const std::string& w);

    struct node
    {
        std::string name;
        size_t i = 0;
        size_t j = 0;
    };

	typedef std::map<std::string, std::vector<std::vector<std::pair<std::vector<node>, bool>>>> dot_matrix_t;
	dot_matrix_t dot_cyk(const std::string& w);

private:
    std::vector<non_terminal>& find_no_terminal(const std::string& a, const std::string& b);
    std::vector<non_terminal> find_terminal(const std::string& t);

public:
    void cyk_dump_dot(const std::string& w, const std::string& path_to_file);

    typedef std::vector<std::tuple<std::string, std::string, std::string>> vector_tuples;
    vector_tuples analyze_graph(graph& graph);

    void cyk_dump_dot(node nt, dot_matrix_t& m, size_t& offset, std::ofstream& out);

    bool cyk_result(const std::string& w);

    void cyk_dump_csv(const std::string& w, const std::string& path_to_file);

	bool satisfy_terminal(const std::vector<expression>& expressions, char c);

    void optimize_grammar();

private:

    bool remove_lvalue(non_terminal& from);

    std::vector<std::pair<non_terminal, std::shared_ptr<token>>> get_terminal_expressions();
    
    void replace_non_terminals(non_terminal& from, std::shared_ptr<token>& to);

	friend std::ostream& operator<<(std::ostream& os, const grammar& gram);

    void grammar_init_hash();

private:
    std::unordered_map<std::string, std::vector<non_terminal>> hash;
	size_t       counter_;
	non_terminal begin_;
	container	 expressions_;
};

