#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "grammar.h"

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
    std::ifstream t(argv[2]);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    str = rtrim(str);
    std::cout << grammar << std::endl;

    //Results
    std::cout << "Result:" << grammar.cyk_result(str) << std::endl;
    grammar.cyk_dump_dot(str, std::string(argv[2]) + ".dot");
    grammar.cyk_dump_csv(str, std::string(argv[2]) + ".csv");
	return 0;
}

