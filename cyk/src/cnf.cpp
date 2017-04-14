#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "grammar.h"

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
    str.resize(str.size() - 1);
    std::cout << grammar << std::endl;
    auto matrix = grammar.cyk(str);
    for (auto & v : matrix)
    {
        std::cout << "***  " << v.first << "  ***" << std::endl;
        for (size_t i = 0; i < v.second.size(); i++)
        {
            for (size_t j = 0; j < v.second.size(); j++)
                std::cout << v.second[i][j] << " ";
            std::cout << std::endl;
        }
    } 
	return 0;
}

