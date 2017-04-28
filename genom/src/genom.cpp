#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "grammar.h"
#include "graph.h"

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
    if (argc != 3)
    {
        std::cerr << "Invalid count arguments" << std::endl;
        return EXIT_FAILURE;
    }
	grammar grammar;
	grammar.load_file(argv[1]);
	grammar.to_cnf();

    graph graph; 
    graph.load_file(argv[2]);
//    graph.generate_graph();
    std::cout << grammar << std::endl;
    std::cout << graph << std::endl;

    grammar::vector_tuples analyze = grammar.analyze_graph(graph);
    for (const auto& t : analyze)
        std::cout << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ")" << std::endl; 

	return 0;
}

