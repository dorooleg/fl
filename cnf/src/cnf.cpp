#include <iostream>
#include "grammar.h"

int main(int argc, char **argv)
{
	using namespace std;
    if (argc != 2)
    {
        std::cerr << "Invalid count arguments" << std::endl;
        return EXIT_FAILURE;
    }
	grammar grammar;
	grammar.load_file(argv[1]);
	grammar.to_cnf();
	cout << grammar;
	return 0;
}

