#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "grammar.h"
#include "graph.h"
#include "graph_analyzer.h"

std::string & ltrim(std::string & str)
{
    auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
    str.erase( str.begin() , it2);
    return str;   
}

std::string & rtrim(std::string & str)
{
    auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
    str.erase( it1.base() , str.end() );
    return str;   
}

int main(int argc, char **argv)
{
	using namespace std;
    if (argc > 3 || argc < 2)
    {
        std::cerr << "Invalid count arguments" << std::endl;
        return EXIT_FAILURE;
    }
	grammar grammar;
	grammar.load_file(argv[1]);
	grammar.to_cnf();
    grammar.optimize_grammar();

    graph graph; 
    if (argc == 2)
    {
        graph.generate_graph();
    }
    else
    {
        graph.load_file(argv[2]);
    }
    std::cout << grammar << std::endl;
    std::cout << graph << std::endl;

    std::cout << "Begin analyze" << std::endl;
    vector_tuples analyze = analyze_graph(grammar, graph);
    for (const auto& t : analyze)
        std::cout << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ")" << std::endl; 

    std::cout << "End analyze" << std::endl;

	return 0;
}

